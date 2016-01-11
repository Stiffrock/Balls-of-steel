// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BOSBall.h"
#include "Bullet4PickUp.generated.h"

UCLASS()
class BOS_API ABullet4PickUp : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *StaticMesh;

	UPROPERTY(EditAnywhere)
		USpotLightComponent *SpotLight;

public:
	// Sets default values for this actor's properties
	ABullet4PickUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void OnBeginOverlap(AActor *OtherActor, class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UPROPERTY(EditAnywhere)
		FTimerHandle respawn;

	void Respawn();

	FVector velocity;
	FVector startLocation;
	bool directionUp;
};
