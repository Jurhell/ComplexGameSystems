// Fill out your copyright notice in the Description page of Project Settings.


#include "WallGravityComponent.h"
#include "PhysicsSystemCharacter.h"

// Sets default values for this component's properties
UWallGravityComponent::UWallGravityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWallGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UWallGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWallGravityComponent::RotatePlayer()
{
}

FVector UWallGravityComponent::GetGroundNormal()
{
	FHitResult Hit = Player->GroundCheck();
	
	if (!Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT(" Player isn't grounded"));
		return FVector();
	}

	Hit.ImpactNormal;
	Hit.GetActor()->GetActorRotation();
	return FVector();
}

FVector UWallGravityComponent::GetWallNormal()
{
	return FVector();
}

void UWallGravityComponent::WallRunning()
{
}

