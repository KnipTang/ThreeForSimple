// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#include "PA_LootChestItem.h"

FInventoryItemHandle::FInventoryItemHandle() :
	HandleID{GetInvalidID()}
{
	
}

FInventoryItemHandle::FInventoryItemHandle(uint32 ID) :
	HandleID{ID}
{
}

FInventoryItemHandle FInventoryItemHandle::CreateHandle()
{
	return FInventoryItemHandle(GenerateNextID());
}

FInventoryItemHandle FInventoryItemHandle::InvalidHandle()
{
	static FInventoryItemHandle InvalidHandle = FInventoryItemHandle();
	return InvalidHandle;
}

bool FInventoryItemHandle::IsValid() const
{
	return HandleID != GetInvalidID();
}

uint32 FInventoryItemHandle::GenerateNextID()
{
	static uint32 StaticID = 1;
	return StaticID++;
}

uint32 FInventoryItemHandle::GetInvalidID()
{
	return 0;
}

bool operator==(const FInventoryItemHandle& Lhs, const FInventoryItemHandle& Rhs)
{
	return Lhs.GetHandleID() == Rhs.GetHandleID();
}

uint32 GetTypeHash(const FInventoryItemHandle& Key)
{
	return Key.GetHandleID();
}

void UInventoryItem::InitItem(const FInventoryItemHandle& NewHandle, const UPA_LootChestItem* NewLootChestItem)
{
	Handle = NewHandle;
	LootChestItem = NewLootChestItem;
}

void UInventoryItem::SetSlotNumber(int NewSlotNumber)
{
	SlotNumber = NewSlotNumber;
}

bool UInventoryItem::IsValid() const
{
	return LootChestItem != nullptr;
}