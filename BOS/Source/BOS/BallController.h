// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BallController.generated.h"

/**
 * 
 */
UCLASS()
class BOS_API ABallController : public APlayerController
{
	GENERATED_BODY()

		ABallController();

public:
	void Respawn();
	void Tick(float DeltaTime) override;

};
