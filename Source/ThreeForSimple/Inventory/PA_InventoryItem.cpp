// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_InventoryItem.h"

FPrimaryAssetId UPA_InventoryItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetInventoryItemAssetType(), GetFName());
}

FPrimaryAssetType UPA_InventoryItem::GetInventoryItemAssetType()
{
	return FPrimaryAssetType("InventoryItem");
}

UTexture2D* UPA_InventoryItem::GetIcon() const
{
	return Icon.LoadSynchronous();
}
