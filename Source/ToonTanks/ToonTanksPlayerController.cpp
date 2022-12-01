// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPLayerEnabled)
{
	// if true enable
	if (bPLayerEnabled) { GetPawn()->EnableInput(this); }
	// else disable
	else { GetPawn()->DisableInput(this); }
}
