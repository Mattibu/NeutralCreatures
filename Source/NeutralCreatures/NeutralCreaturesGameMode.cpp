// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeutralCreaturesGameMode.h"
#include "NeutralCreaturesPlayerController.h"
#include "NeutralCreaturesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeutralCreaturesGameMode::ANeutralCreaturesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ANeutralCreaturesPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}