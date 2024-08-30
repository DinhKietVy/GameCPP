// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPGameMode.h"
#include "CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPPGameMode::ACPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
