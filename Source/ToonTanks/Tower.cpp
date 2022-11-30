// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find distance to tank
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// Check if tank's in range
		if (Distance <= FireRange)
		{
			// If true, rotate turret towards tank
			RotateTurret(Tank->GetActorLocation());
		}
	}
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	// find the player tank for reference later
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}
