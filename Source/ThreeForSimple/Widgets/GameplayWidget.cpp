// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ValueGauge.h"
#include "Items/Inventory/InventoryWidget.h"
#include "Items/LootChest/LootChestWidget.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"
#include "ThreeForSimple/GAS/TfsAttributeSet.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfigureWithASC(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn()));
}

void UGameplayWidget::ConfigureWithASC(class UAbilitySystemComponent* ASC)
{
	if (!ASC)
		return;

	HealthBar->SetAndBoundToGameplayAttribute(ASC, UTfsAttributeSet::GetHealthAttribute(), UTfsAttributeSet::GetMaxHealthAttribute());
	ASC->GenericGameplayEventCallbacks.Add(UTfsAbilitySystemStatics::GetInteractActivateTag()).AddUObject(this, &UGameplayWidget::ToggleLootChest);

}

void UGameplayWidget::ChangeSelectedInventoryItem(float ChangeDirection)
{
	InventoryWidget->ChangeSelectedItem(ChangeDirection);
}

void UGameplayWidget::ToggleDebugGodModeWidget(const struct FGameplayEventData* EventData)
{
	if (DebugGodModeWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		DebugGodModeWidget->SetVisibility(ESlateVisibility::Visible);
		SetShowMouseCursor(true);
		SetFocusToGameAndUI();
		DebugGodModeWidget->SetFocus();
	}
	else
	{
		DebugGodModeWidget->SetVisibility(ESlateVisibility::Hidden);
		SetShowMouseCursor(false);
		SetFocusToGameOnly();
	}
}

void UGameplayWidget::ToggleLootChest(const struct FGameplayEventData* EventData)
{
	if (LootChestWidget->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		LootChestWidget->SetVisibility(ESlateVisibility::Visible);
		PlayLootChestPopupAnimation(true);
		SetShowMouseCursor(true);
		SetFocusToGameAndUI();
		LootChestWidget->SetFocus();
	}
	else
	{
		LootChestWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayLootChestPopupAnimation(false);
		SetShowMouseCursor(false);
		SetFocusToGameOnly();
	}
}

void UGameplayWidget::SetLootChest(class UInventoryWidget* NewLootChestWidget)
{
	LootChestWidget = NewLootChestWidget;
}

void UGameplayWidget::PlayLootChestPopupAnimation(bool bPlayForward)
{
	bPlayForward ? PlayAnimationForward(LootChestPopupAnimation) : PlayAnimationReverse(LootChestPopupAnimation);
}

void UGameplayWidget::SetOwningPawnInputEnabled(bool bPawnInputEnabled)
{
	bPawnInputEnabled ? GetOwningPlayerPawn()->EnableInput(GetOwningPlayer()) : GetOwningPlayerPawn()->DisableInput(GetOwningPlayer());
}

void UGameplayWidget::SetShowMouseCursor(bool bShowMouseCursor)
{
	GetOwningPlayer()->SetShowMouseCursor(bShowMouseCursor);
}

void UGameplayWidget::SetFocusToGameAndUI()
{
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);
	GetOwningPlayer()->SetInputMode(GameAndUIInputMode);
}

void UGameplayWidget::SetFocusToGameOnly()
{
	FInputModeGameOnly GameOnlyInputMode;
	GetOwningPlayer()->SetInputMode(GameOnlyInputMode);
}
