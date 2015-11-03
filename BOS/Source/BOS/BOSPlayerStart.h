// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "BOSPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class BOS_API ABOSPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, Category = "Defaults")
		bool bTeamB;
	
};
