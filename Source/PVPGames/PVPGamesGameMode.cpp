// Copyright Epic Games, Inc. All Rights Reserved.

#include "PVPGamesGameMode.h"
#include "PVPGamesCharacter.h"
#include "UObject/ConstructorHelpers.h"

APVPGamesGameMode::APVPGamesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
