// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentumComponent.h"
#include "PhysicsSystemCharacter.h"

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
	FHitResult Hit = Player->GroundCheck();

	float HitYaw = Hit.GetActor()->GetActorRotation().Yaw;
	float HitPitch = Hit.GetActor()->GetActorRotation().Pitch;
	float HitRoll = Hit.GetActor()->GetActorRotation().Roll;
	
	//Ask about pointers in unreal
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Yaw"), HitYaw));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Pitch"), HitPitch));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Roll"), HitRoll));
}

void UMomentumComponent::IncreaseTopSpeed()
{
}

