// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"

#include "Components/Border.h"
#include "ThreeForSimple/Inventory/InventoryItem.h"
#include "Components/TextBlock.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EmptySlot();
}

void UInventoryItemWidget::UpdateInventoryItem(const class UInventoryItem* Item)
{
	InventoryItem = Item;
	if (!InventoryItem || !InventoryItem->IsValid())
	{
		EmptySlot();
		return;
	}

	SetIcon(Item->GetLootChestItem()->GetIcon());
	UItemToolTip* ToolTip = SetItemToolTipWidget(InventoryItem->GetLootChestItem());

}

bool UInventoryItemWidget::IsEmpty() const
{
	return !InventoryItem || !InventoryItem->IsValid();
}

void UInventoryItemWidget::EmptySlot()
{
	InventoryItem = nullptr;
	SetIcon(EmptyTexture);
	SetToolTip(nullptr);
}

void UInventoryItemWidget::SetSlotNumber(int NewSlotNumber)
{
	SlotNumber = NewSlotNumber;
}

FInventoryItemHandle UInventoryItemWidget::GetItemHandle() const
{
	if (!IsEmpty())
	{
		return InventoryItem->GetHandle();
	}

	return FInventoryItemHandle::InvalidHandle();
}

void UInventoryItemWidget::SetSelected(const FInventoryItemHandle& OldItemHandle, bool bSelectItem)
{
	bSelected = bSelectItem;
	
	bSelected ? Background->SetBrushColor(ColorBackgroundSelected) : Background->SetBrushColor(ColorBackgroundUnselected);

	if (bSelectItem)
		OnItemSelectedDelegate.Broadcast(GetItemHandle(), OldItemHandle);
}
