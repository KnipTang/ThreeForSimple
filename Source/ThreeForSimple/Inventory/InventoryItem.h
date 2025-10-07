// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

//Use hashes to resolve not having to communicate an entire inventory item over the network but only a uint32
USTRUCT()
struct FInventoryItemHandle
{
	GENERATED_BODY()
public:
	FInventoryItemHandle();
	static FInventoryItemHandle CreateHandle();
	static FInventoryItemHandle InvalidHandle();

	bool IsValid() const;
	uint32 GetHandleID() const { return HandleID; };
private:
	explicit FInventoryItemHandle(uint32 ID);

	UPROPERTY()
	uint32 HandleID;

	static uint32 GenerateNextID();
	static uint32 GetInvalidID();
};

bool operator==(const FInventoryItemHandle& Lhs, const FInventoryItemHandle& Rhs);
uint32 GetTypeHash(const FInventoryItemHandle& Key);
/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	void InitItem(const FInventoryItemHandle& NewHandle, const class UPA_LootChestItem* NewLootChestItem);
	const class UPA_LootChestItem* GetLootChestItem() const { return LootChestItem; }

	void SetSlotNumber(int NewSlotNumber);
	FORCEINLINE int GetSlotNumber() const {return SlotNumber; }
	
	//Can be a copy because the size of FInventoryItemHandle is almost the same as a pointer
	FInventoryItemHandle GetHandle() const { return Handle; };

	bool IsValid() const;

	FORCEINLINE void SetSelected(bool bSelectItem = true) {bSelected = bSelectItem; }

	FORCEINLINE bool IsItemSelected() const { return bSelected; }
private:
	UPROPERTY()
	const class UPA_LootChestItem* LootChestItem;
	FInventoryItemHandle Handle;

	int SlotNumber;
	
	bool bSelected;
};
