// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_GASGameMode.h"
#include "TPS_GASCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPS_GASGameMode::ATPS_GASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
