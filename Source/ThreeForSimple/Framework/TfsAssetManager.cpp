// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAssetManager.h"

#include "ThreeForSimple/Inventory/PA_LootChestItem.h"

UTfsAssetManager& UTfsAssetManager::Get()
{
	if (UTfsAssetManager* Singleton = Cast<UTfsAssetManager>(GEngine->AssetManager.Get()))
		return *Singleton;

	UE_LOG(LogLoad, Fatal, TEXT("Asset Manger isn't of type TfsAssetManager"));
	return *NewObject<UTfsAssetManager>();
}

void UTfsAssetManager::LoadChestItems(const FStreamableDelegate& LoadFinishCallBack)
{
	LoadPrimaryAssetsWithType(UPA_LootChestItem::GetLootChestItemAssetType(), TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UTfsAssetManager::LootChestItemsFinishedLoading, LoadFinishCallBack));
}

bool UTfsAssetManager::GetLoadedLootChestItems(TArray<const class UPA_LootChestItem*>& OutItems) const
{
	TArray<UObject*> LoadedObjects;
	bool bLoaded = GetPrimaryAssetObjectList(UPA_LootChestItem::GetLootChestItemAssetType(), LoadedObjects);

	if (bLoaded)
	{
		for (UObject* LoadedObject : LoadedObjects)
			OutItems.Add(Cast<UPA_LootChestItem>(LoadedObject));
	}
		return bLoaded;
}

void UTfsAssetManager::LootChestItemsFinishedLoading(FStreamableDelegate CallBack)
{
	CallBack.ExecuteIfBound();
}
