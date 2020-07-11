// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwinGameMode.h"
#include "TwinPawn.h"

ATwinGameMode::ATwinGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATwinPawn::StaticClass();
}

