// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/Widgets/Items/ItemWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "LootChestItemWidget.generated.h"

/**
 * 
 */
//UserObjectListEntry needed to be able to store this widget in a tileview
UCLASS()
class THREEFORSIMPLE_API ULootChestItemWidget : public UItemWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	//Called when this widget is getting stored in a tileview
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	const class UPA_LootChestItem* GetLootChestItem() const { return LootChestItem; }
private:
	UPROPERTY()
	const class UPA_LootChestItem* LootChestItem;
};
