// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
private:
	void ConfigureWithASC(class UAbilitySystemComponent* ASC);

private:
	UPROPERTY(meta = (BindWidget))
	class UValueGauge* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UCrosshairWidget* CrosshairWidget;

	//Inventory
public:
	void ChangeSelectedInventoryItem(float ChangeDirection);
	//DebugGodModeWidget
public:
	void ToggleDebugGodModeWidget(const struct FGameplayEventData* EventData);

	//Loot Chest
public:
	void ToggleLootChest(const struct FGameplayEventData* EventData);
	void SetLootChest(class UInventoryWidget* NewLootChestWidget);
private:
	void PlayLootChestPopupAnimation(bool bPlayForward);
	void SetOwningPawnInputEnabled(bool bPawnInputEnabled);
	void SetShowMouseCursor(bool bShowMouseCursor);
	void SetFocusToGameAndUI();
	void SetFocusToGameOnly();
	
	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* LootChestWidget;

	UPROPERTY(meta = (BindWidget))
	class ULootChestWidget* DebugGodModeWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* LootChestPopupAnimation;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;
};
