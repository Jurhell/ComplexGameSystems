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
	float TopSpeed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxSpeed = 5000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TopSpeedReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AccelerationRate = 12.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SlopeAcceleration = 25.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
public:	
	// Sets default values for this component's properties
	UMomentumComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = Essential)
	void MomentumBehavior();

	UFUNCTION(BlueprintCallable, Category = Essential)
	void UseMomentum();

	FHitResult GroundCheck();

private:

	float GetSlopeAngle();

	bool bIsPlayerMoving();

	void SlopeMomentum(float SlopeAngle);
	void SlopeBehavior(float SlopeAngle);
	void IncreaseTopSpeed();
	void SpeedCheck();
	void ResetSpeed() { TopSpeed = TopSpeedReset; }
};
