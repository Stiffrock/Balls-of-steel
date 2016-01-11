// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "Bullet4PickUp.h"


// Sets default values
ABullet4PickUp::ABullet4PickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachTo(RootComponent);

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet4PickUp::OnBeginOverlap);


	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->AttachTo(RootComponent);
	SpotLight->bAbsoluteRotation = true;
	//TeamColour->bAbsoluteLocation = true;
	SpotLight->SetRelativeLocationAndRotation(FVector(StaticMesh->GetComponentLocation().X, StaticMesh->GetComponentLocation().Y, StaticMesh->GetComponentLocation().Z), FRotator(-90.f, 0.f, 0.0f));
	SpotLight->SetLightColor(FLinearColor(255.f, 0.f, 255.f, 100.f));
	SpotLight->SetIntensity(10000.f);
	SpotLight->SetAttenuationRadius(1400.f);
	SpotLight->SetOuterConeAngle(60.f);


	velocity.Z = 30;
	directionUp = true;
	startLocation = GetActorLocation();
}

// Called when the game starts or when spawned
void ABullet4PickUp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet4PickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector newLocation = GetActorLocation();
	newLocation += GetActorUpVector()*velocity.Z*DeltaTime;
	SetActorLocation(newLocation, true);

	if (directionUp)
		velocity.Z = 30;
	else
		velocity.Z = -30;

	FVector CheckZVector = GetActorLocation();
	if (CheckZVector.Z > 800 && directionUp)
		directionUp = false;
	else if (CheckZVector.Z < 700 && !directionUp)
		directionUp = true;
}

void ABullet4PickUp::OnBeginOverlap(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABOSBall *bosBall;
	bosBall = Cast<ABOSBall>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (bosBall)
	{
		bosBall->projectile4Count += 3;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 8000));
		GetWorldTimerManager().SetTimer(respawn, this, &ABullet4PickUp::Respawn, 6.0f, false);
	}
}

void ABullet4PickUp::Respawn()
{
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 800));
}