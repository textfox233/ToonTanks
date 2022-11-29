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

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::BeginPlay();

	if (PlayerControllerRef)
	{
		FHitResult HitResult;

		PlayerControllerRef->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);

		DrawDebugSphere(
			GetWorld(),
			//GetActorLocation() + FVector(0.f, 0.f, 200.f),
			HitResult.ImpactPoint,
			25.f,
			12,
			FColor::Red,
			false);
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// WASD
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"),		this, &ATank::Turn);

	// Mouse Movement
	//PlayerInputComponent->BindAxis(TEXT("RotateTurret"),this, &ABasePawn::RotateTurret);
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