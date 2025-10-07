// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, const class UInventoryItem* /*ItemToGrant*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THREEFORSIMPLE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	FOnItemAddedDelegate OnItemAdded;

	void ItemSlotChanged(const FInventoryItemHandle& Handle, int NewSlotNumber);
	UInventoryItem* GetInventoryItemByHandle(const FInventoryItemHandle& Handle) const;

	void TryAddToInventory(const class UPA_LootChestItem* ItemToAdd);

	FORCEINLINE int GetInventoryCapacity() const { return InventoryCapacity; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int InventoryCapacity = 6;
	
	UPROPERTY()
	TMap<struct FInventoryItemHandle, class UInventoryItem*> InventoryMap;

	/***************************************************/
	/*						Server						/
	/***************************************************/
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ItemAdded(const class UPA_LootChestItem* ItemToAdd);

	/***************************************************/
	/*						Client						/
	/***************************************************/
private:
	UFUNCTION(Client, Reliable)
	void Client_ItemAdded(struct FInventoryItemHandle AssignedHandle, const class UPA_LootChestItem* ItemToAdd);
};
