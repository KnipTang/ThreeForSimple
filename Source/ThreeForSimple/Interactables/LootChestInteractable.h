// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TfsInteractable.h"
#include "LootChestInteractable.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ALootChestInteractable : public ATfsInteractable
{
	GENERATED_BODY()

public:
	ALootChestInteractable();
	virtual void BeginPlay() override;

	virtual void OnInteract(AActor* InteractActor) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TArray<class UPA_LootChestItem*> LootChestItems;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UInventoryWidget> LootChestWidgetClass;
	UPROPERTY()
	class UInventoryWidget* LootChestWidget;

	UPROPERTY()
	class UInventoryComponent* InventoryComponent;
};
