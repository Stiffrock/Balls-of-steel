// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BallController.h"
#include "Engine.h"



void ABallController::Respawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball spawned"));
	AGameMode * GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
		Possess(NewPawn);
	}
}
