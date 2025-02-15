// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasicProjectile.generated.h"

UCLASS()
class BOS_API ABasicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasicProjectile();
	virtual void BeginPlay() override;	
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float LifeTime;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float KnockbackImpulse;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float Damage;

	UPROPERTY(EditAnywhere, Category = Movement)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;



	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class USphereComponent* CollisionComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void OnHit_Implementation(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	bool OnHit_Validate(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
