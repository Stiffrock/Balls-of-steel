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

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;



	void CalculateTeams();

	bool ShouldSpawnAtStartSpot(AController* Player) override { return false; };

	bool binitGame;


	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void RespawnAll();

	float delay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		uint8 NumTeamA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		uint8 NumTeamB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		uint8 TeamACount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		uint8 TeamBCount;

};



