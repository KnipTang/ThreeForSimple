// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#include "PA_LootChestItem.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"

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
	PA_LootChestItem = NewLootChestItem;
}

void UInventoryItem::SetSlotNumber(int NewSlotNumber)
{
	SlotNumber = NewSlotNumber;
}

bool UInventoryItem::IsValid() const
{
	return PA_LootChestItem != nullptr;
}

void UInventoryItem::RemoveCurrentAbilityOnSelectedItem(UTfsAbilitySystemComponent* TfsAbilitySystemComponent) const
{
	if (!PA_LootChestItem)
		return;
	if (!TfsAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoASC"));
		return;
	}
	
	switch (PA_LootChestItem->GetItemType())
	{
	case EItemType::Melee:
		break;
	case EItemType::Weapon:
		if (const TSubclassOf<UGameplayAbility> ToRemoveAbility = PA_LootChestItem->GetGrantedInputAbility(ECAbilityInputID::Aim))
			TfsAbilitySystemComponent->RemoveInputAbility(ECAbilityInputID::Aim);
		break;
	}
}

void UInventoryItem::AddCurrentAbilityOnSelectedItem(UTfsAbilitySystemComponent* TfsAbilitySystemComponent) const
{
	if (!PA_LootChestItem)
		return;
	if (!TfsAbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoASC"));
		return;
	}

	switch (PA_LootChestItem->GetItemType())
	{
	case EItemType::Melee:
		break;
	case EItemType::Weapon:
		if (const TSubclassOf<UGameplayAbility> NewAbility = PA_LootChestItem->GetGrantedInputAbility(ECAbilityInputID::Aim))
			TfsAbilitySystemComponent->AddInputAbility(ECAbilityInputID::Aim, NewAbility);
		break;
	}
	
}
