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

// Called every frame
void UMomentumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

//Handles the behavior for the player's movement speed when going up or down slopes
void UMomentumComponent::SlopeMomentum()
{
	float SlopeAngle = GetSlopeAngle();
	FHitResult Hit = GroundCheck();

	if (!Hit.bBlockingHit)
		return;

	//If slope is steeper than 20 degrees cut the player's speed in half
	if (SlopeAngle > 20.0f)
	{
		CurrentSpeed = CurrentSpeed *= 0.5f;
		return;
	}
	
	FVector PlayerForward = Player->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(Hit.ImpactNormal, PlayerForward);

	//CurrentSpeed = FMath::FInterpTo(CurrentSpeed, SlopeAngle, DeltaTime, SlopeAcceleration);
	
	IncreaseTopSpeed(SlopeAngle);

	//Keeping the player's speed in check
	SpeedCheck();
}

FHitResult UMomentumComponent::GroundCheck()
{
	// FHitResult will hold all data returned by line collision query
	FHitResult Hit;

	//Setting a trace from the player mesh's current location to 5cm beneath them
	FVector TraceStart = Player->GetMesh()->GetComponentLocation();
	FVector TraceEnd = (Player->GetActorUpVector() * -10.0f) + Player->GetMesh()->GetComponentLocation();

	//Setting trace to ignore player
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	//Debugging tools
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Green : FColor::Red, false, 5.0f, 0, 10.0f);

	return Hit;
}

void UMomentumComponent::MomentumBehavior()
{
	if (!bIsPlayerMoving())
		return;

	CurrentSpeed += AccelerationRate;

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, "Moving");

	SpeedCheck();
}

void UMomentumComponent::UseMomentum(float PlayerMaxSpeed)
{
	PlayerMaxSpeed = CurrentSpeed;
}

/// <summary>
/// Casts a line at the player's feat to find the normal of what the player is standing on and findss the angle of what's beneath the player.
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

void UMomentumComponent::SlopeRotation()
{
	FHitResult Hit = GroundCheck();

	if (!Hit.bBlockingHit)
		return;

	FVector FloorNormal = Hit.ImpactNormal;
	FVector PlayerUpVector = Player->GetActorUpVector();

	//FVector RotationAxis = FVector::CrossProduct(PlayerUpVector, FloorNormal);
	//RotationAxis.Normalize();

	//float DotProduct = FVector::DotProduct(PlayerUpVector, FloorNormal);
	//float RotationAngle = acosf(DotProduct);

	//FQuat Quaternion = FQuat(RotationAxis, RotationAngle);
	//FQuat RootQuaternion = Player->GetActorQuat(); //If something goes wrong

	//FQuat NewQuat = Quaternion * RootQuaternion;
	//Player->SetActorRotation(NewQuat.Rotator());
}

/// <summary>
/// Increases the player's top movement speed by the entered amount.
/// </summary>
/// <param name="IncreaseAmount">The amount the top speed will increase</param>
void UMomentumComponent::IncreaseTopSpeed(float IncreaseAmount)
{
	//When using the slope angle as the increase amount make the number positive
	if (IncreaseAmount < 0.0f)
		IncreaseAmount *= -1.0f;

	TopSpeed += IncreaseAmount;

	//Keeping the player's speed in check
	SpeedCheck();
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

bool UMomentumComponent::bIsPlayerMoving()
{
	if (!Player)
		return false;

	bool bIsFalling = Player->GetCharacterMovement()->IsFalling();
	FVector Velocity = Player->GetCharacterMovement()->Velocity;

	if (!bIsFalling && Velocity.Length() > 0.f)
		return true;

	return false;
}