// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TfsAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTfsAssetManager& Get();
	void LoadChestItems(const FStreamableDelegate& LoadFinishCallBack);
	bool GetLoadedLootChestItems(TArray<const class UPA_LootChestItem*>& OutItems) const;
private:
	void LootChestItemsFinishedLoading(FStreamableDelegate CallBack);
};
