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

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	ABOSBall();

	void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, Category = Ball)
	float MaxDashImpulse;

	UPROPERTY(EditAnywhere, Category = Ball)
	float DashChargeRate;
	float DashImpulse;

	UPROPERTY(EditAnywhere, Category=Ball)
	float JumpImpulse;

	UPROPERTY(EditAnywhere, Category=Ball)
	float RollTorque;
	float CameraLag;

	FVector CurrentDirection;

	bool bCanJump;
	bool dashCharging;
	bool bProjectile_1;
	bool bProjectile_2;
	bool bCameraLag;
	

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
