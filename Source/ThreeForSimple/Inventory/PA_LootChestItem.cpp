// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_LootChestItem.h"

FPrimaryAssetId UPA_LootChestItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetLootChestItemAssetType(), GetFName());
}

FPrimaryAssetType UPA_LootChestItem::GetLootChestItemAssetType()
{
	return FPrimaryAssetType("LootChestItem");
}

UTexture2D* UPA_LootChestItem::GetIcon() const
{
	return Icon.LoadSynchronous();
}

TSubclassOf<class UGameplayAbility> UPA_LootChestItem::GetGrantedInputAbility(const ECAbilityInputID InputID) const
{
	if (const TSubclassOf<class UGameplayAbility>* Ability = GrantedInputAbility.Find(InputID))
		return *Ability;
	return nullptr;
}
