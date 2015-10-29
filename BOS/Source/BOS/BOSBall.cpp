// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSBall.h"
#include "BasicProjectile.h"

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
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
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
	SpringArm->CameraLagSpeed = 3.f;

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BasicProjectile_BP.BasicProjectile_BP'"));
	if (ItemBlueprint.Object){
		ABasicProjectile_BP = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

void ABOSBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (dashCharging)
		DashImpulse += DashChargeRate * DeltaTime;
	DashImpulse = FMath::Clamp(DashImpulse, 0.0f, MaxDashImpulse);
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
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABOSBall::Fire);
}

void ABOSBall::YawCamera(float Val)
{
	FRotator newRotation = SpringArm->GetComponentRotation();
	newRotation.Yaw += Val;
	SpringArm->SetRelativeRotation(newRotation);
}

void ABOSBall::PitchCamera(float Val)
{
	FRotator newRotation = SpringArm->GetComponentRotation();
	newRotation.Pitch = FMath::Clamp(newRotation.Pitch + Val, -80.0f, 80.0f);
	SpringArm->SetRelativeRotation(newRotation);
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

void ABOSBall::Add_Torque_Implementation(FVector torque)
{
	Ball->AddTorque(torque);
}
bool ABOSBall::Add_Torque_Validate(FVector torque)
{
	return true;
}

void ABOSBall::Add_Impulse_Implementation(FVector impulse)
{
	Ball->AddImpulse(impulse);
}
bool ABOSBall::Add_Impulse_Validate(FVector torque)
{
	return true;
}

void ABOSBall::Fire()
{
	const FRotator SpawnRotation = SpringArm->GetComponentRotation();
	const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 200.0f);

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		ABasicProjectile* Projectile = World->SpawnActor<ABasicProjectile>(ABasicProjectile_BP, SpawnLocation, SpawnRotation, SpawnParams);

		//World->SpawnActor<AMyProjectile>(AMyProjectile::StaticClass(), SpawnLocation, SpawnRotation);
	}

}
