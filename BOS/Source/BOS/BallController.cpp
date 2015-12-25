// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BallController.h"
#include "Engine.h"
#include "BOSBall.h"
#include "BOSGameMode.h"
#include "BOSGameState.h"
#include "BOSPlayerState.h"

ABallController::ABallController()
{
	/*AGameMode * GameMode = GetWorld()->GetAuthGameMode();
	APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
	Possess(NewPawn);*/
}

void ABallController::Respawn()
{
	AGameMode * GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		ABOSGameState* gameState = Cast<ABOSGameState>(GetWorld()->GameState);
		ABOSGameMode* bosmode = Cast<ABOSGameMode>(GameMode);

		APawn* thisPawn = GetPawn();
		thisPawn->Destroy();
		

		APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
		Possess(NewPawn);

		ABOSPlayerState * PS = Cast<ABOSPlayerState>(NewPawn->PlayerState);

		if (PS->bTeamB && bosmode->binitGame == true)
		{
			gameState->TeamALives += 1;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TeamB"));
		}
		else if (bosmode->binitGame == true)
		{
			gameState->TeamBLives += 1;
		}
	
	}
}

void ABallController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}