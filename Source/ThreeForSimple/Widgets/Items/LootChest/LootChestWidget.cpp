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

	if (APawn* OwnerPawn = GetOwningPlayerPawn())
		OwnerInventoryComponent = OwnerPawn->GetComponentByClass<UInventoryComponent>();

	LoadLootChestItems();
	LootChestItemList->OnEntryWidgetGenerated().AddUObject(this, &ULootChestWidget::LootChestItemWidgetGenerated);
}

void ULootChestWidget::SetLootChestItems(const TArray<UPA_LootChestItem*>& AllItems)
{
	LootChestItems = AllItems;


	//PopulateItemMap();
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
	{
		//if (LootChestItems.Contains(LootChestItem))
			//Const casting because there is no reason for this not to be const, "bypassing" the unreal missing const version for this function
			LootChestItemList->AddItem(const_cast<UPA_LootChestItem*>(LootChestItem));
	}
}

void ULootChestWidget::LootChestItemWidgetGenerated(UUserWidget& NewWidget)
{
	if (ULootChestItemWidget* ItemWidget = Cast<ULootChestItemWidget>(&NewWidget))
	{
		if (OwnerInventoryComponent)
			ItemWidget->OnItemTaken.AddUObject(OwnerInventoryComponent, &UInventoryComponent::TryAddToInventory);
		ItemWidget->OnItemTaken.AddUObject(this, &ULootChestWidget::RemoveItemFromLootChest);

		ItemsMap.Add(ItemWidget->GetLootChestItem());
	}

	UE_LOG(LogTemp, Display, TEXT("LootChestItemListGENERATED"));
}

void ULootChestWidget::PopulateItemMap()
{
	//const TArray<UUserWidget*>& UserWidgets = LootChestItemList->GetDisplayedEntryWidgets();
	//for (const UUserWidget* UserWidget : UserWidgets)
	//{
	//	if (ULootChestItemWidget* ItemWidget = Cast<ULootChestItemWidget>(&UserWidget))
	//	{
	//		if (OwnerInventoryComponent)
	//			ItemWidget->OnItemTaken.AddUObject(OwnerInventoryComponent, &UInventoryComponent::TryAddToInventory);
	//		ItemWidget->OnItemTaken.AddUObject(this, &ULootChestWidget::RemoveItemFromLootChest);
	//		ItemsMap.Add(ItemWidget->GetLootChestItem());
	//	}
	//}
}

void ULootChestWidget::RemoveItemFromLootChest(const UPA_LootChestItem* ItemToRemove)
{
	if (ItemToRemove)
	{
		LootChestItemList->RemoveItem(const_cast<UPA_LootChestItem*>(ItemToRemove));
		ItemsMap.Remove(ItemToRemove);
	}
}
