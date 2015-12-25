// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BOSPlayerState.h"
#include "UnrealNetwork.h"
#include "BallController.h"


void ABOSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABOSPlayerState, bTeamB);
	
}

