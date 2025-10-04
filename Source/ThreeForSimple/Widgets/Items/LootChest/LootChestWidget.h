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

private:
	void LoadLootChestItems();
	void LootChestItemLoadFinished();
	void LootChestItemWidgetGenerated(UUserWidget& NewWidget);
	
	UPROPERTY(meta = (BindWidget))
	class UTileView* LootChestItemList;

	UPROPERTY()
	TMap<const class UPA_LootChestItem*, const class ULootChestWidget*> ItemsMap;
};
