// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// COMPONENT CREATION
	// ------------------

	// Mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	// Movement Handler
	MovementHandler = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Handler"));
	MovementHandler->InitialSpeed = 1300.f;
	MovementHandler->MaxSpeed = 1300.f;

	// Particle System - Smoke Trail
	SmokeTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	SmokeTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		// play launch sound effect
		UGameplayStatics::PlaySoundAtLocation(
			this,				// World context object
			LaunchSound,		// Actual sound effect
			GetActorLocation()	// Location
		);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) return;

	// Gather data for apply damage function
	// -------------------------------------
	 
	// EventInstigator - Controller that was responsible for causing this damage (e.g. player who shot the weapon)
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	
	// DamageTypeClass - Class that describes the damage that was done.
	UClass* DamageTypeClass = UDamageType::StaticClass();

	// Apply damage (if the actor hit isn't null, isn't this projectile and isn't this projectile's owner)
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,			// DamagedActor - Actor that will be damaged.
			Damage,				// BaseDamage - The base damage to apply.
			MyOwnerInstigator,	// EventInstigator - Controller that was responsible for causing this damage (e.g. player who shot the weapon)
			this,				// DamageCauser - Actor that actually caused the damage (e.g. the grenade that exploded)
			DamageTypeClass		// DamageTypeClass - Class that describes the damage that was done.
		);

		// Spawn a hit explosion
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				this,						// World context object
				HitParticles,				// Particle system
				this->GetActorLocation(),	// Location
				this->GetActorRotation()	// Rotation
			);
		}
		
		// play launch sound effect
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,			// World context object
				HitSound,		// Actual sound effect
				Hit.Location	// Location
			);
		}
		
		// shake the camera
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}

		// Destroy projectile last
		Destroy();
	}
}
