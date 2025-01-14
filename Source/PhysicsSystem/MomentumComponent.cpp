// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentumComponent.h"
#include "PhysicsSystemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	// ...
	
}


// Called every frame
void UMomentumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMomentumComponent::SlopeMomentum()
{

	float SlopeAngle = Player->GroundCheckSlopes();

	if (SlopeAngle == 0)
	{
		ResetSpeed();
		return;
	}

	double DeltaTime = GetWorld()->DeltaTimeSeconds;

	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, SlopeAngle, DeltaTime, SlopeAcceleration);

	IncreaseTopSpeed();

	if (CurrentSpeed >= TopSpeed)
		CurrentSpeed = TopSpeed;

	//FString S = FString::SanitizeFloat();

	/*GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, *S);*/
}

void UMomentumComponent::IncreaseTopSpeed()
{
}