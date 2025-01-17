// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MomentumComponent.generated.h"

class APhysicsSystemCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PHYSICSSYSTEM_API UMomentumComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TopSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TopSpeedReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AccelerationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SlopeAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector StoredVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bStoreMomentum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
public:	
	// Sets default values for this component's properties
	UMomentumComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SlopeMomentum();
	FHitResult GroundCheck();

private:
	UFUNCTION(BlueprintCallable, Category = Essential)
	void MomentumBehavior();

	UFUNCTION(BlueprintCallable, Category = Essential)
	void UseMomentum(float PlayerMaxSpeed);

	float GetSlopeAngle();
	void SlopeRotation();

	bool bIsPlayerMoving();

	void IncreaseTopSpeed(float IncreaseAmount);
	void SpeedCheck();
	void ResetSpeed() { TopSpeed = TopSpeedReset; }
};
