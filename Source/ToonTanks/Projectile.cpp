// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	MovementHandler = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Handler"));
	MovementHandler->InitialSpeed = 1300.f;
	MovementHandler->MaxSpeed = 1300.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("HitComp: %s"),		*HitComp->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"),	*OtherActor->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"),		*OtherComp->GetName());

	auto MyOwner = GetOwner();
	if (MyOwner == nullptr) return;

	// Gather data for apply damage function
	// -------------------------------------
	 
	// EventInstigator - Controller that was responsible for causing this damage (e.g. player who shot the weapon)
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	
	// DamageTypeClass - Class that describes the damage that was done.
	auto DamageTypeClass = UDamageType::StaticClass();

	// Apply damage
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,			// DamagedActor - Actor that will be damaged.
			Damage,				// BaseDamage - The base damage to apply.
			MyOwnerInstigator,	// EventInstigator - Controller that was responsible for causing this damage (e.g. player who shot the weapon)
			this,				// DamageCauser - Actor that actually caused the damage (e.g. the grenade that exploded)
			DamageTypeClass		// DamageTypeClass - Class that describes the damage that was done.
		);
	}

	// Destroy projectile AFTER applying damage
	Destroy();
}
