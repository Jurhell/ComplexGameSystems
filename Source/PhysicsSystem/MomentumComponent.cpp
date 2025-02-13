// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentumComponent.h"
#include "PhysicsSystemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UMomentumComponent::UMomentumComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMomentumComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	TopSpeedReset = TopSpeed;
}

/// <summary>
/// Handles all the behavior for the component
/// </summary>
void UMomentumComponent::MomentumBehavior()
{
	//Checks if we are moving
	if (!bIsPlayerMoving())
	{
		//Decrease the current speed if not
		CurrentSpeed -= AccelerationRate * 3;
		return;
	}

	//Increasing speed while moving
	CurrentSpeed += AccelerationRate;

	float SlopeAngle = GetSlopeAngle();

	//Constantly performing slope behavior
	SlopeMomentum(SlopeAngle);

	//Keeping speed in check
	SpeedCheck();
}

//Handles the behavior for the player's movement speed when going up or down slopes
void UMomentumComponent::SlopeMomentum(float SlopeAngle)
{
	FHitResult Hit = GroundCheck();

	if (!Hit.bBlockingHit)
		return;

	//If player is not on a slope
	if (SlopeAngle < 10.f && SlopeAngle > -10.f)
	{
		//Reset their speed
		ResetSpeed();
		return;
	}

	SlopeBehavior(SlopeAngle);

	FVector PlayerForward = Player->GetActorForwardVector();
	
	float DotProduct = FVector::DotProduct(Hit.ImpactNormal, PlayerForward);
	
	float Target = MovementCurve->GetFloatValue(DotProduct);
	float DeltaSeconds = GetWorld()->DeltaTimeSeconds / 4;
	
	float InterpResult = FMath::FInterpTo(CurrentSpeed, Target, DeltaSeconds, SlopeAcceleration);
	CurrentSpeed = InterpResult;

	//Testing
	FString S = FString::SanitizeFloat(InterpResult);
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, *S);

	//If player reaches top speed while descending a slope increase top speed
	if (CurrentSpeed <= TopSpeed)
		IncreaseTopSpeed();
}

//Some extra behavior for when the player is on a slope
void UMomentumComponent::SlopeBehavior(float SlopeAngle)
{
	if (SlopeAngle > -10.f)
		return;

	//If the player's speed is at or above the default top speed
	if (CurrentSpeed <= TopSpeedReset)
	{
		//Begin increasing speed instead of using float curve
		CurrentSpeed += SlopeAcceleration;
		return;
	}
}

FHitResult UMomentumComponent::GroundCheck()
{
	// FHitResult will hold all data returned by line collision query
	FHitResult Hit;

	//Setting a trace from the player mesh's current location to 5cm beneath them
	FVector TraceStart = Player->GetMesh()->GetComponentLocation();
	FVector TraceEnd = (Player->GetActorUpVector() * -20.0f) + Player->GetMesh()->GetComponentLocation();

	//Setting trace to ignore player
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	return Hit;
}

/// <summary>
/// Subsitutes the default max speed with the one from this component
/// </summary>
/// <param name="PlayerMaxSpeed">The Character Movement component's max walk speed</param>
void UMomentumComponent::UseMomentum()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

	//FVector PlayerForward = Player->GetActorForwardVector().GetSafeNormal();
	//FVector Force = PlayerForward * (CurrentSpeed * 2);

	//Player->GetCharacterMovement()->AddForce(Force);
}

/// <summary>
/// Casts a line at the player's feat to find the normal of what the player is standing on and finds the angle of what's beneath the player.
/// </summary>
/// <returns>The angle of the slope</returns>
float UMomentumComponent::GetSlopeAngle()
{
	FHitResult Hit = GroundCheck();

	if (!Hit.bBlockingHit)
		return 0.f;

	//Storing variables for use in GetSlopeDegreeAngles
	FVector FloorNormal = Hit.ImpactNormal;
	FVector PlayerForward = Player->GetActorForwardVector();
	FVector PlayerUpVector = Player->GetActorUpVector();
	float OutPitch;
	float OutAngle;

	UKismetMathLibrary::GetSlopeDegreeAngles(PlayerForward, FloorNormal, PlayerUpVector, OutPitch, OutAngle);

	return OutAngle;
}

/// <summary>
/// Increases the player's top movement speed by the entered amount.
/// </summary>
/// <param name="IncreaseAmount">The amount the top speed will increase</param>
void UMomentumComponent::IncreaseTopSpeed()
{
	TopSpeed = MaxSpeed;
}

//Checks if the player's current and top speeds are exceeding the limits set for them and clamps them within those limits
void UMomentumComponent::SpeedCheck()
{
	//Making sure the current speed doesn't drop below 0
	if (CurrentSpeed < 0.f)
		CurrentSpeed = 0.f;

	//Making sure the current speed doesn't exceed the top speed
	if (CurrentSpeed >= TopSpeed)
		CurrentSpeed = TopSpeed;

	//Making sure the player's top speed doesn't exceed the max speed
	if (TopSpeed >= MaxSpeed)
		TopSpeed = MaxSpeed;
}

/// <summary>
/// Checks if the player is moving
/// </summary>
/// <returns>Whether the player is moving or not</returns>
bool UMomentumComponent::bIsPlayerMoving()
{
	if (!Player)
		return false;

	bool bIsFalling = Player->GetCharacterMovement()->IsFalling();
	FVector Velocity = Player->GetCharacterMovement()->Velocity;

	//Check if player is walking or jumping
	if (!bIsFalling && Velocity.Length() > 0.f ||
		bIsFalling && Velocity.Length() > 0.f)
		return true;

	return false;
}