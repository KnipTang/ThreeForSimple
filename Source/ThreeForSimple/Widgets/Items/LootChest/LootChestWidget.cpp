// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestWidget.h"

#include "LootChestItemWidget.h"
#include "ThreeForSimple/Framework/TfsAssetManager.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"
#include "Components/TileView.h"

void ULootChestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	LoadLootChestItems();
	LootChestItemList->OnEntryWidgetGenerated().AddUObject(this, &ULootChestWidget::LootChestItemWidgetGenerated);
}

void ULootChestWidget::LoadLootChestItems()
{
	UTfsAssetManager::Get().LoadChestItems(FStreamableDelegate::CreateUObject(this, &ULootChestWidget::LootChestItemLoadFinished));
}

void ULootChestWidget::LootChestItemLoadFinished()
{
	TArray<const UPA_LootChestItem*> LootChestItems;
	UTfsAssetManager::Get().GetLoadedLootChestItems(LootChestItems);
	for (const UPA_LootChestItem* LootChestItem : LootChestItems)
		//Const casting because there is no reason for this not to be const, "bypassing" the unreal missing const version for this function
		LootChestItemList->AddItem(const_cast<UPA_LootChestItem*>(LootChestItem));
}

void ULootChestWidget::LootChestItemWidgetGenerated(UUserWidget& NewWidget)
{
	if (ULootChestItemWidget* ItemWidget = Cast<ULootChestItemWidget>(&NewWidget))
		ItemsMap.Add(ItemWidget->GetLootChestItem());
}
