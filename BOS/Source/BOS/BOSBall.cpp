// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSBall.h"
#include "BasicProjectile.h"
#include "Engine.h"
#include "BallController.h"

ABOSBall::ABOSBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 8000.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->bDoCollisionTest = true;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->CameraLagMaxDistance = 50.0f;


	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	MaxDashImpulse = 3000000.0f;
	DashChargeRate = 500000.0f;
	DashImpulse = 0.0f;
	bCanJump = true; // Start being able to jump
	dashCharging = false;
	bIsDead = false;
	Health = 100;



	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BasicProjectile_BP.BasicProjectile_BP'"));
	if (ItemBlueprint.Object)
		ABasicProjectile_BP = (UClass*)ItemBlueprint.Object->GeneratedClass;	

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint2(TEXT("Blueprint'/Game/BasicProjectile_BP2.BasicProjectile_BP2'"));
	if (ItemBlueprint2.Object)
		ABasicProjectile_BP2 = (UClass*)ItemBlueprint2.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint3(TEXT("Blueprint'/Game/BasicProjectile_BP3.BasicProjectile_BP3'"));
	if (ItemBlueprint3.Object)
		ABasicProjectile_BP3 = (UClass*)ItemBlueprint3.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint4(TEXT("Blueprint'/Game/BasicProjectile_BP4.BasicProjectile_BP4'"));
	if (ItemBlueprint4.Object)
		ABasicProjectile_BP4 = (UClass*)ItemBlueprint4.Object->GeneratedClass;
	
	bProjectile_1 = true;
	bProjectile_2 = false;
	bProjectile_3 = false;
	bProjectile_4 = false;
}

void ABOSBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (dashCharging)
	{
		DashImpulse += DashChargeRate * DeltaTime;
		DashImpulse = FMath::Clamp(DashImpulse, 0.0f, MaxDashImpulse);
	}
}

void ABOSBall::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings

	//Bind Axis
	InputComponent->BindAxis("YawCamera", this, &ABOSBall::YawCamera);
	InputComponent->BindAxis("PitchCamera", this, &ABOSBall::PitchCamera);
	InputComponent->BindAxis("MoveForward", this, &ABOSBall::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABOSBall::MoveRight);

	//Bind Action
	InputComponent->BindAction("Dash", IE_Pressed, this, &ABOSBall::DashCharge);
	InputComponent->BindAction("Dash", IE_Released, this, &ABOSBall::DashRelease);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ABOSBall::Jump);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABOSBall::Server_Fire);
	InputComponent->BindAction("SwitchToProjectile_1", IE_Pressed, this, &ABOSBall::SetProjectile_1);
	InputComponent->BindAction("SwitchToProjectile_2", IE_Pressed, this, &ABOSBall::SetProjectile_2);
	InputComponent->BindAction("SwitchToProjectile_3", IE_Pressed, this, &ABOSBall::SetProjectile_3);
	InputComponent->BindAction("SwitchToProjectile_4", IE_Pressed, this, &ABOSBall::SetProjectile_4);

}

void ABOSBall::HandleDeath_Implementation()
{
	bIsDead = true;
	ABallController* BC = Cast<ABallController>(GetController());
	BC->Respawn();
	this->Destroy();
}
bool ABOSBall::HandleDeath_Validate()
{
	return true;
}

void ABOSBall::HandleCameraLag(float DeltaTime)
{	
	if (SpringArm->CameraLagSpeed >= 0.0f)
	{
		SpringArm->CameraLagSpeed -= 0.25f * DeltaTime;
	}
	
	else
	{
		bCameraLag = false;
		SpringArm->bEnableCameraLag = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EndCameraLag"), CameraLag));
	}
		
}

void ABOSBall::YawCamera(float Val)
{
	FRotator newRotation = SpringArm->GetComponentRotation();
	newRotation.Yaw += Val;
	SpringArm->SetRelativeRotation(newRotation);
	Add_YawCamera(newRotation);
}

void ABOSBall::Add_YawCamera_Implementation(FRotator rotator) //Server function
{
	SpringArm->SetRelativeRotation(rotator);
}

bool ABOSBall::Add_YawCamera_Validate(FRotator rotator) //Server function
{
	return true;
}

void ABOSBall::PitchCamera(float Val)
{	
	FRotator newRotation = SpringArm->GetComponentRotation();
	newRotation.Pitch = FMath::Clamp(newRotation.Pitch + Val, -80.0f, 80.0f);
	SpringArm->SetRelativeRotation(newRotation);
	Add_PitchCamera(newRotation);
}

void ABOSBall::Add_PitchCamera_Implementation(FRotator rotator) //Server function
{
	SpringArm->SetRelativeRotation(rotator);
}

bool ABOSBall::Add_PitchCamera_Validate(FRotator rotator) //Server function
{
	return true;
}

void ABOSBall::MoveRight(float Val)
{
	FVector Direction = SpringArm->GetForwardVector();
	Direction.Z = 0;
	const FVector Torque = Direction * -FMath::Clamp(Val, -1.0f, 1.0f) * RollTorque;
	Add_Torque(Torque);
}

void ABOSBall::MoveForward(float Val)
{
	FVector Direction = SpringArm->GetRightVector();
	Direction.Z = 0;
	const FVector Torque = Direction * 	FMath::Clamp(Val, -1.0f, 1.0f) * RollTorque;
	Add_Torque(Torque);
}

void ABOSBall::DashCharge()
{
	dashCharging = true;
}

void ABOSBall::DashRelease()
{
	const FVector Impulse = SpringArm->GetForwardVector() * DashImpulse;
	Add_Impulse(Impulse);
	DashImpulse = 0.0f;
	dashCharging = false;
}

void ABOSBall::Jump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
	if(bCanJump)
	{

		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Add_Impulse(Impulse);	
		bCanJump = false;
	}
}

void ABOSBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

void ABOSBall::Add_Torque_Implementation(FVector torque) //Server functionv
{
	Ball->AddTorque(torque);
}
bool ABOSBall::Add_Torque_Validate(FVector torque) //Server function
{
	return true;
}

void ABOSBall::Add_Impulse_Implementation(FVector impulse) //Server function
{
	Ball->AddImpulse(impulse);
}
bool ABOSBall::Add_Impulse_Validate(FVector torque) //Server function
{
	return true;
}

void ABOSBall::Server_Fire_Implementation() //Server function
{
	const FRotator SpawnRotation = SpringArm->GetComponentRotation();
	const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 200.0f);

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;

		if (bProjectile_1)
		{
			ABasicProjectile* Projectile = World->SpawnActor<ABasicProjectile>(ABasicProjectile_BP, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else if (bProjectile_2)
		{
			ABasicProjectile* Projectile = World->SpawnActor<ABasicProjectile>(ABasicProjectile_BP2, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else if (bProjectile_3)
		{
			ABasicProjectile* Projectile = World->SpawnActor<ABasicProjectile>(ABasicProjectile_BP3, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else if (bProjectile_4)
		{
			ABasicProjectile* Projectile = World->SpawnActor<ABasicProjectile>(ABasicProjectile_BP4, SpawnLocation, SpawnRotation, SpawnParams);
		}

		//World->SpawnActor<AMyProjectile>(AMyProjectile::StaticClass(), SpawnLocation, SpawnRotation); Creates the class from scratch
	}
}

bool ABOSBall::Server_Fire_Validate() //Server function 
{
	return true;
}

void ABOSBall::SetProjectile_1_Implementation() //Server function
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PROJECTILE_1_ACTIVATED"));
	bProjectile_1 = true;
	bProjectile_2 = false;
	bProjectile_3 = false;
	bProjectile_4 = false;

}

bool ABOSBall::SetProjectile_1_Validate() //Server function
{
	return true;
}

void ABOSBall::SetProjectile_2_Implementation() //Server function
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PROJECTILE_2_ACTIVATED"));
	bProjectile_1 = false;
	bProjectile_2 = true;
	bProjectile_3 = false;
	bProjectile_4 = false;
}

bool ABOSBall::SetProjectile_2_Validate() //Server function
{
	return true;
}

void ABOSBall::SetProjectile_3_Implementation() //Server function
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PROJECTILE_3_ACTIVATED"));
	bProjectile_1 = false;
	bProjectile_2 = false;
	bProjectile_3 = true;
	bProjectile_4 = false;
}

bool ABOSBall::SetProjectile_3_Validate() //Server function
{
	return true;
}

void ABOSBall::SetProjectile_4_Implementation() //Server function
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PROJECTILE_4_ACTIVATED"));
	bProjectile_1 = false;
	bProjectile_2 = false;
	bProjectile_3 = false;
	bProjectile_4 = true;
}

bool ABOSBall::SetProjectile_4_Validate() //Server function
{
	return true;
}

void ABOSBall::Damage_Implementation(uint32 damage)
{
	Health -= damage;
	if (Health <= 0)
		HandleDeath();
}

bool ABOSBall::Damage_Validate(uint32 damage)
{
	return true;
}