// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

// Health component reports a 0 hp actor
void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (Tank->GetTankPlayerController())
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
			//Tank->DisableInput(Tank->GetTankPlayerController());
			//Tank->GetTankPlayerController()->bShowMouseCursor = false;
		}
	}
	if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
	}
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
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
