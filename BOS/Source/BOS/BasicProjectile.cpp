// Fill out your copyright notice in the Description page of Project Settings.

#include "BOS.h"
#include "BasicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"
#include "BOSBall.h"



ABasicProjectile::ABasicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachTo(RootComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.0f;
	// Die after 3 seconds by default
	//InitialLifeSpan = 5.0f;
	LifeTime = 5.0f;
	InitialLifeSpan = LifeTime;

}

void ABasicProjectile::OnHit_Implementation(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit detected!"));
		OtherComp->AddImpulseAtLocation(GetVelocity() * KnockbackImpulse, GetActorLocation());

		ABOSBall* Ball = Cast<ABOSBall>(OtherActor); // Access the playerball from projectile class. Ball pointer to a cast for OtherActor to BOSBall?

		if (OtherActor == Ball)
		{
			Ball->Damage(10.f);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball Hit!" Ball->Health));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ball Hit! Health: %f"), Ball->Health));
		}


	//	Destroy();
	}
}

bool ABasicProjectile::OnHit_Validate(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return true;
}



void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ABasicProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

