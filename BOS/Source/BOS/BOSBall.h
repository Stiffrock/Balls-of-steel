// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "BOSBall.generated.h"

UCLASS(config=Game)
class ABOSBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent *TeamColour;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	ABOSBall();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Add_Torque(FVector torque);	
	void Add_Torque_Implementation(FVector torque);
	bool Add_Torque_Validate(FVector torque);

	UFUNCTION(Server, Reliable, WithValidation)
	void Add_Impulse(FVector impulse);
	void Add_Impulse_Implementation(FVector torque);
	bool Add_Impulse_Validate(FVector torque);

	UFUNCTION(Server, Reliable, WithValidation)
	void Add_YawCamera(FRotator rotator);
	void Add_YawCamera_Implementation(FRotator rotator);
	bool Add_YawCamera_Validate(FRotator rotator);

	UFUNCTION(Server, Reliable, WithValidation)
		void Add_PitchCamera(FRotator rotator);
	void Add_PitchCamera_Implementation(FRotator rotator);
	bool Add_PitchCamera_Validate(FRotator rotator);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();
	void Server_Fire_Implementation();
	bool Server_Fire_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetProjectile_1();
	void SetProjectile_1_Implementation();
	bool SetProjectile_1_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetProjectile_2();
	void SetProjectile_2_Implementation();
	bool SetProjectile_2_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetProjectile_3();
	void SetProjectile_3_Implementation();
	bool SetProjectile_3_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetProjectile_4();
	void SetProjectile_4_Implementation();
	bool SetProjectile_4_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void HandleDeath();
	void HandleDeath_Implementation();
	bool HandleDeath_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Damage(float damage);
	void Damage_Implementation(float damage);
	bool Damage_Validate(float damage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball)
	float MaxDashImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball)
	float DashImpulse;

	UPROPERTY(EditAnywhere, Category = Ball)
	float DashChargeRate;

	UPROPERTY(EditAnywhere, Category = Ball)
	float JumpImpulse;

	UPROPERTY(EditAnywhere, Category = Ball)
	float RollTorque;

	UPROPERTY(EditAnywhere, Category = Ball)
	float ImpulseSpeed;

	float CameraLag;

	float intensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball)
	float Health;

	bool bCanJump;
	bool dashCharging;
	UPROPERTY(BlueprintReadOnly)
	bool bProjectile_1;
	UPROPERTY(BlueprintReadOnly)
	bool bProjectile_2;
	UPROPERTY(BlueprintReadOnly)
	bool bProjectile_3;
	UPROPERTY(BlueprintReadOnly)
	bool bProjectile_4;
	bool bCameraLag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball)
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly)
	int32 projectile4Count;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle projectileCooldown;
	bool projectileAvailable;
	void projectileCooldownReset();

protected:
	void YawCamera(float Val);
	void PitchCamera(float Val);

	void HandleCameraLag(float DeltaTime);

	void DashCharge();
	void DashRelease();

	void MoveRight(float Val);
	void MoveForward(float Val);

	void Jump();


	UPROPERTY()
	TSubclassOf<class ABasicProjectile> ABasicProjectile_BP;

	UPROPERTY()
	TSubclassOf<class ABasicProjectile> ABasicProjectile_BP2;

	UPROPERTY()
	TSubclassOf<class ABasicProjectile> ABasicProjectile_BP3;

	UPROPERTY()
	TSubclassOf<class ABasicProjectile> ABasicProjectile_BP4;

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
