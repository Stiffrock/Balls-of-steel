// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BOSGameState.h"
#include "UnrealNetwork.h"
#include "Engine.h"



ABOSGameState::ABOSGameState()
{

}

void ABOSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABOSGameState, TeamALives);
	DOREPLIFETIME(ABOSGameState, TeamBLives);

	if (TeamALives >= 5)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("TEAM B WINS"));
	}
	if (TeamBLives >= 5)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("TEAM B WINS"));
	}

}
void ABOSGameState::Tick(float DeltaTime)
{
	if (TeamALives >= 5)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("TEAM B WINS"));
	}
	if (TeamBLives >= 5)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("TEAM B WINS"));
	}
}