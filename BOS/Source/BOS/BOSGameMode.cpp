// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSGameMode.h"
#include "BOSBall.h"

ABOSGameMode::ABOSGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ABOSBall::StaticClass();
}
