// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "BOSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BOS_API ABOSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
		UPROPERTY(Replicated, BlueprintReadOnly, Category = "Defaults")
		bool bTeamB;
};
