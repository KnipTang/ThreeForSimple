// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootChestWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ULootChestWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetLootChestItems(const TArray<class UPA_LootChestItem*>& AllItems);
private:
	void LoadLootChestItems();
	void LootChestItemLoadFinished();
	void LootChestItemWidgetGenerated(UUserWidget& NewWidget);

	void PopulateItemMap();

	void RemoveItemFromLootChest(const class UPA_LootChestItem* ItemToRemove);

	int Capacity = 20;
	
	UPROPERTY(meta = (BindWidget))
	class UTileView* LootChestItemList;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TArray<class UPA_LootChestItem*> LootChestItems;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<class ULootChestItemWidget> ItemWidgetClass;
	
	UPROPERTY()
	TArray<class ULootChestItemWidget*> ItemWidgets;
	
	UPROPERTY()
	TMap<const class UPA_LootChestItem*, const class ULootChestWidget*> ItemsMap;

	UPROPERTY()
	class UInventoryComponent* OwnerInventoryComponent;
};
