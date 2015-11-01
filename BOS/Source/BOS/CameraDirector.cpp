// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"


// Sets default values
ACameraDirector::ACameraDirector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CameraArray.Add(CameraOne);
	//CameraArray.Add(CameraTwo);

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float TimeBetweenCameraChanges = 15.0f;
	const float SmoothBlendTime = 9.0f;
	TimeToNextCameraChange -= DeltaTime;

	if (TimeToNextCameraChange <= 0.0f)
	{
		TimeToNextCameraChange += TimeBetweenCameraChanges;

		//Find actor that handles ctonrtol for local player.

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (OurPlayerController)
		{

			if (CameraTwo && (OurPlayerController->GetViewTarget() == CameraOne))
			{
				//Blend smoothly to camera two.
				OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
			}
			else if (CameraOne)
			{
				//Cut instantly to camera one.
				//OurPlayerController->SetViewTarget(CameraOne);
				OurPlayerController->SetViewTargetWithBlend(CameraOne, SmoothBlendTime);
			}
		}

	}

}

