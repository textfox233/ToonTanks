// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create root capsule
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	// create base mesh and attach to root capsule
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	// create turret mesh and attach to base
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// create projectile spawn point and attach to turret
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	// TODO: visual/sound effects

	// Explosion! :o
	if (ExplosionDeathParticles)
	{
		// Spawn a death explosion
		UGameplayStatics::SpawnEmitterAtLocation(
			this,						// World context object
			ExplosionDeathParticles,	// Particle system
			this->GetActorLocation(),	// Location
			this->GetActorRotation()	// Rotation
		);
	}
	if (DeathSound)
	{
		// Play death sound effect
		UGameplayStatics::PlaySoundAtLocation(
			this,				// World context object
			DeathSound,			// Actual sound effect
			GetActorLocation()	// Location
		);
	}
	if (DeathCameraShakeClass)
	{
		// Shake the camera
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("no shakey")); }
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// get line between here and the target
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	// get rotation (just the yaw, no leaning wanted)
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	// set rotation
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.f
		)
	);
}

void ABasePawn::Fire()
{
	// spawn a projectile
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		ProjectileSpawnPoint->GetComponentLocation(), 
		ProjectileSpawnPoint->GetComponentRotation()
		);

	Projectile->SetOwner(this);

	//DrawDebugSphere(
	//	GetWorld(),
	//	ProjectileSpawnPoint->GetComponentLocation(),
	//	25.f,
	//	12,
	//	FColor::Red,
	//	false,
	//	3.f
	//);
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
