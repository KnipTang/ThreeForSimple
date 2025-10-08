// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/Widgets/Items/ItemWidget.h"
#include "InventoryItemWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemSelectedDelegate, const struct FInventoryItemHandle& /*NewItemHande*/, const struct FInventoryItemHandle& /*OldItemHande*/)

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UInventoryItemWidget : public UItemWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FOnItemSelectedDelegate OnItemSelectedDelegate;
	
	void UpdateInventoryItem(const class UInventoryItem* Item);
	bool IsEmpty() const;
	void EmptySlot();
	void SetSlotNumber(int NewSlotNumber);
	FORCEINLINE int GetSlotNumber() const { return SlotNumber; }

	FORCEINLINE const UInventoryItem* GetInventoryItem() const {return InventoryItem;}
	FInventoryItemHandle GetItemHandle() const;
	
	void SetSelected(const FInventoryItemHandle& OldItemHandle, bool bSelectItem = true);
	FORCEINLINE bool IsItemSelected() const { return bSelected; }
private:
	UPROPERTY(meta = (BindWidget))
	class UBorder* Background;
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	class UTexture2D* EmptyTexture;
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FLinearColor ColorBackgroundUnselected;
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FLinearColor ColorBackgroundSelected;
	
	UPROPERTY()
	const UInventoryItem* InventoryItem;

	int SlotNumber;

	bool bSelected;
};
