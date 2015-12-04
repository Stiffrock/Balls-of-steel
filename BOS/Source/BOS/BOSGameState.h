// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "BOSGameState.generated.h"

/**
 * 
 */
UCLASS()
class BOS_API ABOSGameState : public AGameState
{
	GENERATED_BODY()

		ABOSGameState();
	
public:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = Defaults)
		uint8 TeamALives;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = Defaults)
		uint8 TeamBLives;
};
