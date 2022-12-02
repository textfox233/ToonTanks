// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank :: ATank()
{
	// construct components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// set up attachments
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::BeginPlay();

	if (TankPlayerController)
	{
		FHitResult HitResult;

		TankPlayerController->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);

		//DrawDebugSphere(
		//	GetWorld(),
		//	HitResult.ImpactPoint,
		//	25.f,
		//	12,
		//	FColor::Blue,
		//	false);
	}
}

// What happens on actor destruction
void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	// hide and disable, don't destroy
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	// mark the tank as dead
	bAlive = false;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// WASD
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"),		this, &ATank::Turn);

	// Fire Turret (on button press)
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value)
{
	FVector DeltaLocation(0.f);

	// Modify X by speed and delta time
	DeltaLocation.X = (Value * Speed) * UGameplayStatics::GetWorldDeltaSeconds(this);

	// Move along according to delta location
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation(0.f);

	// Modify Yaw by turn rate and delta time
	DeltaRotation.Yaw = (Value * TurnRate) * UGameplayStatics::GetWorldDeltaSeconds(this);

	// Rotate according to delta rotation
	AddActorLocalRotation(DeltaRotation, true);
}