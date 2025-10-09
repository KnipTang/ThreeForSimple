// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsGameState.h"

#include "Net/UnrealNetwork.h"
#include "ThreeForSimple/Player/PlayerInfoTypes.h"

void ATfsGameState::UpdatePlayerSelection(const APlayerState* PlayerToUpdate, const uint8 DesiredSlot)
{
	if (!HasAuthority() || IsSlotOccupied(DesiredSlot))
		return;

	FPlayerSelection* FoundExistingPlayerSelection = PlayerSelectionArray.FindByPredicate([&](const FPlayerSelection& PlayerSelection)
		{
			return PlayerSelection.IsPlayerLinkedToSelection(PlayerToUpdate);
		}
	);

	if (FoundExistingPlayerSelection)
	{
		FoundExistingPlayerSelection->SetSlotID(DesiredSlot);
	}
	else
	{
		PlayerSelectionArray.Add(FPlayerSelection(DesiredSlot, PlayerToUpdate));
	}

	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}

bool ATfsGameState::IsSlotOccupied(const uint8 SlotId) const
{
	for (const FPlayerSelection& PlayerSelection : PlayerSelectionArray)
	{
		if (PlayerSelection.GetSlotID() == SlotId)
		{
			return true;
		}
	}

	return false;
}

bool ATfsGameState::CanStartGame() const
{
	return PlayerSelectionArray.Num() == PlayerArray.Num();
}

const TArray<FPlayerSelection>& ATfsGameState::GetPlayerSelection() const
{
	return PlayerSelectionArray;
}

void ATfsGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ATfsGameState, PlayerSelectionArray, COND_None, REPNOTIFY_Always);
}

void ATfsGameState::OnRep_PlayerSelectionArray()
{
	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}