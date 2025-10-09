// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "GameFramework/PlayerState.h"
#include "ThreeForSimple/Framework/TfsGameInstance.h"
#include "ThreeForSimple/Framework/TfsGameState.h"

void ALobbyPlayerController::Server_RequestSlotSelectionChange_Implementation(uint8 NewSlotID) const
{
	if (!GetWorld())
		return;

	ATfsGameState* TfsGameState = GetWorld()->GetGameState<ATfsGameState>();
	if (!TfsGameState)
		return;

	TfsGameState->UpdatePlayerSelection(GetPlayerState<APlayerState>(), NewSlotID);
}

bool ALobbyPlayerController::Server_RequestSlotSelectionChange_Validate(uint8 NewSlotID)
{
	return true;	
}

void ALobbyPlayerController::Server_RequestStartGame_Implementation()
{
	UTfsGameInstance* TfsGameInstance = GetGameInstance<UTfsGameInstance>();
	if (TfsGameInstance)
		TfsGameInstance->StartGame();
}

bool ALobbyPlayerController::Server_RequestStartGame_Validate()
{
	return true;
}
