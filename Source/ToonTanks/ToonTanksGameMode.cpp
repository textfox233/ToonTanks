// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

// Health component reports a 0 hp actor
void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	// if the dead actor is the player tank:
	if (DeadActor == Tank)
	{
		// 1. Destroy the tank
		Tank->HandleDestruction();

		// 2. Disable the player controller
		if (Tank->GetTankPlayerController()) { ToonTanksPlayerController->SetPlayerEnabledState(false); }

		// 3. Trigger gameover with a loss
		GameOver(false);
	}
	// if the dead actor is a CPU tower:
	if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		// 1. Destroy the tower
		DestroyedTower->HandleDestruction();

		// 2. Update number of active towers within the level
		TargetTowers--;

		// 3. If there are no more towers to destroy trigger gameover with a victory
		if (TargetTowers == 0) { GameOver(true); }
	}
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

//void AToonTanksGameMode::GameOver(bool bWonGame)
//{
//
//}

void AToonTanksGameMode::HandleGameStart()
{
	// Get the number of active towers within the level
	TargetTowers = GetTargetTowerCount();

	// Get the player
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Get the player controller
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	// Disable player
	if (ToonTanksPlayerController) 
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		// set a timer
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,								// User Object where Callback Function exists
			&AToonTanksPlayerController::SetPlayerEnabledState,		// Callback Function Address
			true													// Any Inputs for the Function
		);
		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,	// Handle
			PlayerEnableTimerDelegate,	// Delegate
			StartDelay,					// Delay
			false						// Loop?
		);
	}
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	// get all active towers within the level and store them in out variable AllTowers
	TArray<AActor*> AllTowers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), AllTowers);

	return AllTowers.Num();
}
