// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInfoTypes.generated.h"

USTRUCT()
struct FPlayerSelection
{
	GENERATED_BODY()
public:
	FPlayerSelection();
	FPlayerSelection(uint8 InSlot, const class APlayerState* InPlayerState);

	FORCEINLINE uint8 GetSlotID() const { return SlotID; };
	FORCEINLINE void SetSlotID(const uint8 NewSlotID) { SlotID = NewSlotID; };
	FORCEINLINE FUniqueNetIdRepl GetPlayerUniqueId() const { return PlayerUniqueId; };
	FORCEINLINE FText GetPlayerNickName() const { return PlayerNickName; };

	//Check if passed in player state is linked to this player selection
	bool IsPlayerLinkedToSelection(const APlayerState* InPlayerState) const;
	
	static uint8 GetInvalidSlot();
private:
	UPROPERTY()
	uint8 SlotID;

	UPROPERTY()
	FUniqueNetIdRepl PlayerUniqueId;

	UPROPERTY()
	FText PlayerNickName;
};