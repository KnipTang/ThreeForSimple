// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ValueGauge.h"
#include "Items/LootChest/LootChestWidget.h"
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
}

void UGameplayWidget::ToggleLootChest()
{
	if (LootChestWidget->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		LootChestWidget->SetVisibility(ESlateVisibility::Visible);
		PlayLootChestPopupAnimation(true);
	}
	else
	{
		LootChestWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayLootChestPopupAnimation(false);	
	}
}

void UGameplayWidget::PlayLootChestPopupAnimation(bool bPlayForward)
{
	bPlayForward ? PlayAnimationForward(LootChestPopupAnimation) : PlayAnimationReverse(LootChestPopupAnimation);
}
