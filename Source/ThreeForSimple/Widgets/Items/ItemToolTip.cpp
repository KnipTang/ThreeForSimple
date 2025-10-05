// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTip.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"

void UItemToolTip::SetItem(const class UPA_LootChestItem* Item)
{
	ItemIcon->SetBrushFromTexture(Item->GetIcon());
	ItemTitleText->SetText(Item->GetItemName());
	ItemDescriptionText->SetText(Item->GetItemDescription());
}
