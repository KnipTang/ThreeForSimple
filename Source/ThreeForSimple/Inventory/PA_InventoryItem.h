// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PA_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UPA_InventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetInventoryItemAssetType();
	UTexture2D* GetIcon() const;

	FText GetItemName() const { return ItemName; }
	FText GetItemDescription() const { return ItemDescription; }
	float GetPrice() const { return Price; }

	bool GetIsConsumable() const { return bIsConsumable; }
	
	TSubclassOf<class UGameplayEffect> GetEquippedEffect() const { return EquippedEffect; }
	TSubclassOf<class UGameplayEffect> GetConsumeEffect() const { return ConsumeEffect; }
	TSubclassOf<class UGameplayAbility> GetGrantedAbility() const { return GrantedAbility; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	float Price;
	
	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	bool bIsConsumable;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TSubclassOf<class UGameplayEffect> EquippedEffect;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TSubclassOf<class UGameplayEffect> ConsumeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryItem")
	TSubclassOf<class UGameplayAbility> GrantedAbility;
};
