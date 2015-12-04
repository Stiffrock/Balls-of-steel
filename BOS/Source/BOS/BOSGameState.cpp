// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BOSGameState.h"
#include "UnrealNetwork.h"


ABOSGameState::ABOSGameState()
{

}

void ABOSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABOSGameState, TeamALives);
	DOREPLIFETIME(ABOSGameState, TeamBLives);
}