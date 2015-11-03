// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSGameMode.h"
#include "BOSBall.h"
#include "BOSPlayerState.h"
#include "BOSPlayerStart.h"

ABOSGameMode::ABOSGameMode()
{
	// set default pawn class to our ball
	// = ABOSBall::StaticClass();


	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BOSBall_BP.BOSBall_BP'"));
	if (ItemBlueprint.Object){
		DefaultPawnClass = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}


	PlayerStateClass = ABOSPlayerState::StaticClass();
}

void ABOSGameMode::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		ABOSPlayerState * PS = Cast<ABOSPlayerState>(NewPlayer->PlayerState);

		if (PS && GameState)
		{
			uint8 NumTeamA = 0;
			uint8 NumTeamB = 0;	

		/*	for (APlayerState * It : GameState)
			{
				ABOSPlayerState* OtherPS = Cast<ABOSPlayerState>(It);

				if (OtherPS->bTeamB)
				{
					NumTeamB++;
				}
				else
				{
					NumTeamA++;
				}
			}
			if (NumTeamA > NumTeamB)
			{
				PS->bTeamB = true;
			}*/
		}
	}
}


AActor* ABOSGameMode::ChoosePlayerStart(AController* Player)
{
	/*if (Player)
	{
		ABOSPlayerState * PS = Cast<ABOSPlayerState>(Player->PlayerState);

		if (PS)
		{
			TArray<ABOSPlayerStart *> Starts;
			for (TActorIterator<ABOSPlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
			{
				if (StartItr->bTeamB == PS->bTeamB)
				{
					Starts.Add(*StartItr);
				}
			}

			return Starts[FMath::RandRange(0, Starts.Num - 1)];
		}
	}*/

	return NULL;
}