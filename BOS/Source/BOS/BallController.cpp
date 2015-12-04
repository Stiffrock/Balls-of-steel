// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BallController.h"
#include "Engine.h"
#include "BOSBall.h"
#include "BOSPlayerState.h"

ABallController::ABallController()
{

}

void ABallController::Respawn()
{
	AGameMode * GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		APawn * NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
		Possess(NewPawn);

		ABOSPlayerState * PS = Cast<ABOSPlayerState>(NewPawn->PlayerState);
		if (PS->bTeamB)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TeamB"));
		}
	}
}

void ABallController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball destroyed"));
	//ABOSBall* ball = Cast<ABOSBall>(GetPawn());
	//if (ball->bIsDead)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball destroyed"));
	//	//ball->Destroy();
	//}
}