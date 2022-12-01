// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the tank's in range, rotate turret towards tank
	if (TankInRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

// What happens on actor destruction
void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	// find the player tank for reference later
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
	// If the tank's in range, fire
	if (TankInRange())
	{
		Fire();
	}
}

bool ATower::TankInRange()
{
	if (Tank)
	{
		// Find distance to tank
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// Check if tank's in range
		return Distance <= FireRange;
	}
	return false;
}
