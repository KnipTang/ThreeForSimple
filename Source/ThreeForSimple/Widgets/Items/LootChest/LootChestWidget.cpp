// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestWidget.h"
#include "LootChestItemWidget.h"
#include "ThreeForSimple/Framework/TfsAssetManager.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"
#include "Components/TileView.h"
#include "ThreeForSimple/Inventory/InventoryComponent.h"

void ULootChestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	LoadLootChestItems();
	LootChestItemList->OnEntryWidgetGenerated().AddUObject(this, &ULootChestWidget::LootChestItemWidgetGenerated);
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
		OwnerInventoryComponent = OwnerPawn->GetComponentByClass<UInventoryComponent>();
}

void ULootChestWidget::SetLootChestItems(class TArray<UPA_LootChestItem*> Items)
{
	LootChestItems = Items;
}

void ULootChestWidget::LoadLootChestItems()
{
	UTfsAssetManager::Get().LoadChestItems(FStreamableDelegate::CreateUObject(this, &ULootChestWidget::LootChestItemLoadFinished));
}

void ULootChestWidget::LootChestItemLoadFinished()
{
	TArray<const UPA_LootChestItem*> AllLootChestItems;
	UTfsAssetManager::Get().GetLoadedLootChestItems(AllLootChestItems);
	for (const UPA_LootChestItem* LootChestItem : AllLootChestItems)
		if (LootChestItems.Contains(LootChestItem))
		//Const casting because there is no reason for this not to be const, "bypassing" the unreal missing const version for this function
		LootChestItemList->AddItem(const_cast<UPA_LootChestItem*>(LootChestItem));
}

void ULootChestWidget::LootChestItemWidgetGenerated(UUserWidget& NewWidget)
{
	if (ULootChestItemWidget* ItemWidget = Cast<ULootChestItemWidget>(&NewWidget))
	{
		if (OwnerInventoryComponent)
			ItemWidget->OnItemTaken.AddUObject(OwnerInventoryComponent, &UInventoryComponent::TryAddToInventory);
		
		ItemsMap.Add(ItemWidget->GetLootChestItem());
	}

	UE_LOG(LogTemp, Display, TEXT("LootChestItemListGENERATED"));
}
