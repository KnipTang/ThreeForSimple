// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TfsGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSelectionUpdated, const TArray<struct FPlayerSelection>& /*NewPlayerSelection*/);
/**
 * 
 */
UCLASS()
class ATfsGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void UpdatePlayerSelection(const class APlayerState* PlayerToUpdate, const uint8 DesiredSlot);
	bool IsSlotOccupied(const uint8 SlotId) const;

	bool CanStartGame() const;

	FOnPlayerSelectionUpdated OnPlayerSelectionUpdated;
	
	const TArray<struct FPlayerSelection>& GetPlayerSelection() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
private:	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSelectionArray)
	TArray<struct FPlayerSelection> PlayerSelectionArray;

	UFUNCTION()
	void OnRep_PlayerSelectionArray();
};
