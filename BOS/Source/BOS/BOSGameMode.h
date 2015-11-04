// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "BOSGameMode.generated.h"

UCLASS(minimalapi)
class ABOSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABOSGameMode();

	UPROPERTY()
		TSubclassOf<class ABOSBall> ABOSBall_BP;

	void PostLogin(APlayerController* NewPlayer) override;

	AActor* ChoosePlayerStart(APlayerController* Player);

	bool ShouldSpawnAtStartSpot(AController* Player) override { return false; };

};



