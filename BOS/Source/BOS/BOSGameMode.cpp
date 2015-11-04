// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSGameMode.h"
#include "BOSBall.h"
#include "BOSPlayerState.h"
#include "BOSPlayerStart.h"
#include "BOSGameState.h"
#include "BallController.h"
#include "Engine.h"

ABOSGameMode::ABOSGameMode()
{
	// set default pawn class to our ball
	// = ABOSBall::StaticClass();


	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BOSBall_BP.BOSBall_BP'"));
	if (ItemBlueprint.Object){
		DefaultPawnClass = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

	GameStateClass = ABOSGameState::StaticClass();
	PlayerStateClass = ABOSPlayerState::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();

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
		
			for (APlayerState * It : GameState->PlayerArray)
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
			}
		}
		ChoosePlayerStart(NewPlayer);
	}
}


AActor* ABOSGameMode::ChoosePlayerStart(APlayerController* Player)
{

	if (Player)
	{
		ABOSPlayerState * PS = Cast<ABOSPlayerState>(Player->PlayerState);

		if (PS)
		{
			TArray<ABOSPlayerStart *> Starts;	

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start added"));
			for (TObjectIterator<ABOSPlayerStart> StartItr; StartItr; ++StartItr)
			{

				if (StartItr->bTeamB == PS->bTeamB)
				{
					Starts.Add(*StartItr);
		
				}
			}
			return Starts[0];
			//return Starts[FMath::RandRange(0, Starts.Num - 1)];
		}
	}

	return NULL;
}