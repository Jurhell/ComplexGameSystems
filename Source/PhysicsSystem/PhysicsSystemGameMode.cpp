// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsSystemGameMode.h"
#include "PhysicsSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

APhysicsSystemGameMode::APhysicsSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
