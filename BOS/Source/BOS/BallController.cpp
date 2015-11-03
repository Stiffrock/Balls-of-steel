// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BallController.h"



void ABallController::Respawn()
{
	AGameMode * GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
		Possess(NewPawn);
	}
}
