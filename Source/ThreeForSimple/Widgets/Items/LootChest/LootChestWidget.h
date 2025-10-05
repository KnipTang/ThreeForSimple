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

	void SetLootChestItems(class TArray<class UPA_LootChestItem*> Items);
private:
	void LoadLootChestItems();
	void LootChestItemLoadFinished();
	void LootChestItemWidgetGenerated(UUserWidget& NewWidget);
	
	UPROPERTY(meta = (BindWidget))
	class UTileView* LootChestItemList;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	class TArray<UPA_LootChestItem*> LootChestItems;
	
	UPROPERTY()
	TMap<const class UPA_LootChestItem*, const class ULootChestWidget*> ItemsMap;

	UPROPERTY()
	class UInventoryComponent* OwnerInventoryComponent;
};
