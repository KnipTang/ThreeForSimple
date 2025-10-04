// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestItemWidget.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"

void ULootChestItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	LootChestItem = Cast<UPA_LootChestItem>(ListItemObject);
	if (!LootChestItem)
		return;

	SetIcon(LootChestItem->GetIcon());
}
