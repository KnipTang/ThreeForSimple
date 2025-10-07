// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThreeForSimple/Inventory/InventoryItem.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UInventoryComponent* InventoryComponent;
private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* ItemsContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<class UInventoryItemWidget> ItemWidgetClass;

	UPROPERTY()
	TArray<UInventoryItemWidget*> ItemWidgets;
	UPROPERTY()
	TMap<FInventoryItemHandle, UInventoryItemWidget*> PopulatedItemEntryWidgets;

	void ItemAdded(const UInventoryItem* InventoryItem);

	UInventoryItemWidget* GetNextAvailableSlot() const;

	//CurrentSelectedItem
public:
	void ChangeSelectedItem(float ChangeDirection);
private:
	UPROPERTY()
	UInventoryItemWidget* SelectedItemWidgets;
};
