// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkingProjectGameMode.h"
#include "NetworkingProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetworkingProjectGameMode::ANetworkingProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
