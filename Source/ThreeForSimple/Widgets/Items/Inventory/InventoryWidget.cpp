// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryItemWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"
#include "ThreeForSimple/GAS/TfsGameplayAbilityTypes.h"
#include "ThreeForSimple/Inventory/InventoryComponent.h"
#include "ThreeForSimple/Inventory/PA_LootChestItem.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APawn* OwnerPawn = GetOwningPlayerPawn())
	{
		InventoryComponent = OwnerPawn->GetComponentByClass<UInventoryComponent>();
		if (InventoryComponent)
		{
			InventoryComponent->OnItemAdded.AddUObject(this, &UInventoryWidget::ItemAdded);
			const int Capacity = InventoryComponent->GetInventoryCapacity();

			OwnerAbilitySystemComponent = Cast<UTfsAbilitySystemComponent>(InventoryComponent->GetOwnerAbilitySystemComponent());
			
			ItemsContainer->ClearChildren();
				
			for (int i = 0; i < Capacity; ++i)
			{
				if (UInventoryItemWidget* NewEmptyWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), ItemWidgetClass))
				{
					NewEmptyWidget->SetSlotNumber(i);
					UWrapBoxSlot* NewItemSlot = ItemsContainer->AddChildToWrapBox(NewEmptyWidget);
					NewItemSlot->SetPadding(FMargin(2.f));
					ItemWidgets.Add(NewEmptyWidget);
				}
			}
		}
	}
}

void UInventoryWidget::ItemAdded(const UInventoryItem* InventoryItem)
{
	if (!InventoryItem)
		return;

	if (UInventoryItemWidget* NextAvailableSlot = GetNextAvailableSlot())
	{
		NextAvailableSlot->UpdateInventoryItem(InventoryItem);
		PopulatedItemEntryWidgets.Add(InventoryItem->GetHandle(), NextAvailableSlot);
		if (InventoryComponent)
		{
			InventoryComponent->ItemSlotChanged(InventoryItem->GetHandle(), NextAvailableSlot->GetSlotNumber());
		}
		if (!SelectedItemWidgets)
		{
			SelectedItemWidgets = NextAvailableSlot;
			SelectedItemWidgets->SetSelected(true);
		}
	}
}

UInventoryItemWidget* UInventoryWidget::GetNextAvailableSlot() const
{
	for (UInventoryItemWidget* Widget : ItemWidgets)
	{
		if (Widget->IsEmpty())
		{
			return Widget;
		}
	}

	return nullptr;
}

void UInventoryWidget::ChangeSelectedItem(float ChangeDirection)
{
	int NewSelectedSlotNumber = SelectedItemWidgets->GetSlotNumber() + ChangeDirection;
	if (NewSelectedSlotNumber > PopulatedItemEntryWidgets.Num() - 1 || NewSelectedSlotNumber < 0)
		return;

	SelectedItemWidgets->SetSelected(false);
	RemoveCurrentAbilityOnSelectedItem();
	
	SelectedItemWidgets = ItemWidgets[NewSelectedSlotNumber];
	SelectedItemWidgets->SetSelected(true);
	AddCurrentAbilityOnSelectedItem();
}

void UInventoryWidget::RemoveCurrentAbilityOnSelectedItem()
{
	const UPA_LootChestItem* PA_Item = SelectedItemWidgets->GetInventoryItem()->GetLootChestItem();
	if (!PA_Item)
		return;

	OwnerAbilitySystemComponent = Cast<UTfsAbilitySystemComponent>(InventoryComponent->GetOwnerAbilitySystemComponent());
	
	switch (PA_Item->GetItemType())
	{
	case EItemType::Melee:
		if (!OwnerAbilitySystemComponent)
			break;
		if (const TSubclassOf<UGameplayAbility> NewAbility = PA_Item->GetGrantedInputAbility(ECAbilityInputID::BasicAttack))
			OwnerAbilitySystemComponent->RemoveInputAbility(ECAbilityInputID::BasicAttack, NewAbility);
		break;
	case EItemType::Weapon:
		if (!OwnerAbilitySystemComponent)
			break;
		if (const TSubclassOf<UGameplayAbility> NewAbility = PA_Item->GetGrantedInputAbility(ECAbilityInputID::Aim))
			OwnerAbilitySystemComponent->RemoveInputAbility(ECAbilityInputID::Aim, NewAbility);
		break;
	}
}

void UInventoryWidget::AddCurrentAbilityOnSelectedItem()
{
	const UPA_LootChestItem* PA_Item = SelectedItemWidgets->GetInventoryItem()->GetLootChestItem();
	if (!PA_Item)
		return;

	OwnerAbilitySystemComponent = Cast<UTfsAbilitySystemComponent>(InventoryComponent->GetOwnerAbilitySystemComponent());
	
	switch (PA_Item->GetItemType())
	{
	case EItemType::Melee:
		if (!OwnerAbilitySystemComponent)
			break;
		if (const TSubclassOf<UGameplayAbility> NewAbility = PA_Item->GetGrantedInputAbility(ECAbilityInputID::BasicAttack))
			OwnerAbilitySystemComponent->AddInputAbility(ECAbilityInputID::BasicAttack, NewAbility);
		break;
	case EItemType::Weapon:
		if (!OwnerAbilitySystemComponent)
			break;
		if (const TSubclassOf<UGameplayAbility> NewAbility = PA_Item->GetGrantedInputAbility(ECAbilityInputID::Aim))
			OwnerAbilitySystemComponent->AddInputAbility(ECAbilityInputID::Aim, NewAbility);
		break;
	}
	
}
