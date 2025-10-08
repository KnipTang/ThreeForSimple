// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PA_LootChestItem.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::ItemSlotChanged(const FInventoryItemHandle& Handle, int NewSlotNumber)
{
	if (UInventoryItem* FoundItem = GetInventoryItemByHandle(Handle))
	{
		FoundItem->SetSlotNumber(NewSlotNumber);
	}
}

UInventoryItem* UInventoryComponent::GetInventoryItemByHandle(const FInventoryItemHandle& Handle) const
{
	UInventoryItem* const* FoundItem = InventoryMap.Find(Handle);
	if (FoundItem)
	{
		return *FoundItem;
	}

	return nullptr;
}

void UInventoryComponent::TryAddToInventory(const UPA_LootChestItem* ItemToAdd)
{
	UE_LOG(LogTemp, Display, TEXT("Inventory Item Added"));

	Server_ItemAdded(ItemToAdd);
}

void UInventoryComponent::TryAddCurrentAbilityOnSelectedItem(const FInventoryItemHandle& NewItemHandle, const FInventoryItemHandle& OldItemHandle)
{
	if (!GetInventoryItemByHandle(NewItemHandle))
		return;
	
	Server_AddCurrentAbilityOnSelectedItem(NewItemHandle, OldItemHandle);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerTfsAbilitySystemComponent = Cast<UTfsAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
	
}

void UInventoryComponent::Server_AddCurrentAbilityOnSelectedItem_Implementation(const FInventoryItemHandle& NewItemHandle, const FInventoryItemHandle& OldItemHandle)
{
	UInventoryItem* CurrentItem = GetInventoryItemByHandle(NewItemHandle);
	UInventoryItem* OldItem = GetInventoryItemByHandle(OldItemHandle);
	if (!CurrentItem)
		return;

	if (!GetOwner()->HasAuthority())
		return;

	if (OldItem)
		OldItem->RemoveCurrentAbilityOnSelectedItem(OwnerTfsAbilitySystemComponent);
	CurrentItem->AddCurrentAbilityOnSelectedItem(OwnerTfsAbilitySystemComponent);
}

bool UInventoryComponent::Server_AddCurrentAbilityOnSelectedItem_Validate(const FInventoryItemHandle& NewItemHandle, const FInventoryItemHandle& OldItemHandle)
{
	return true;
}

void UInventoryComponent::Server_ItemAdded_Implementation(const UPA_LootChestItem* ItemToAdd)
{
	if (!GetOwner()->HasAuthority())
		return;

	if (InventoryMap.Num() >= GetInventoryCapacity())
		return;

	UInventoryItem* InventoryItem = NewObject<UInventoryItem>();
	FInventoryItemHandle NewHandle = FInventoryItemHandle::CreateHandle();
	InventoryItem->InitItem(NewHandle, ItemToAdd);
	InventoryMap.Add(NewHandle, InventoryItem);
	
	UE_LOG(LogTemp, Warning, TEXT("Server Adding Shop Item: %s, with Id: %d"), *(InventoryItem->GetLootChestItem()->GetItemName().ToString()), NewHandle.GetHandleID());

	OnItemAdded.Broadcast(InventoryItem);
	
	Client_ItemAdded(NewHandle, ItemToAdd);
}

bool UInventoryComponent::Server_ItemAdded_Validate(const UPA_LootChestItem* ItemToAdd)
{
	return true;
}

void UInventoryComponent::Client_ItemAdded_Implementation(struct FInventoryItemHandle AssignedHandle,
	const UPA_LootChestItem* ItemToAdd)
{
	if (GetOwner()->HasAuthority())
		return;

	UInventoryItem* InventoryItem = NewObject<UInventoryItem>();
	InventoryItem->InitItem(AssignedHandle, ItemToAdd);
	InventoryMap.Add(AssignedHandle, InventoryItem);
	
	OnItemAdded.Broadcast(InventoryItem);

	UE_LOG(LogTemp, Warning, TEXT("Server Adding Shop Item: %s, with Id: %d"), *(InventoryItem->GetLootChestItem()->GetItemName().ToString()), AssignedHandle.GetHandleID());

}
