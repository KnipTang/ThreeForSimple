// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PA_LootChestItem.generated.h"

enum class ECAbilityInputID : uint8;
/**
 * 
 */
UENUM()
enum class EItemType
{
	Melee,
	Weapon
		
};

UCLASS()
class THREEFORSIMPLE_API UPA_LootChestItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetLootChestItemAssetType();
	UTexture2D* GetIcon() const;

	EItemType GetItemType() const { return ItemType; }
	
	FText GetItemName() const { return ItemName; }
	FText GetItemDescription() const { return ItemDescription; }

	bool GetIsConsumable() const { return bIsConsumable; }
	
	TSubclassOf<class UGameplayEffect> GetEquippedEffect() const { return EquippedEffect; }
	TSubclassOf<class UGameplayEffect> GetConsumeEffect() const { return ConsumeEffect; }
	TSubclassOf<class UGameplayAbility> GetGrantedNormalAbility() const { return GrantedNormalAbility; }
	TSubclassOf<class UGameplayAbility> GetGrantedInputAbility(const ECAbilityInputID InputID) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	bool bIsConsumable;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	TSubclassOf<class UGameplayEffect> EquippedEffect;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	TSubclassOf<class UGameplayEffect> ConsumeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	TSubclassOf<class UGameplayAbility> GrantedNormalAbility;

	UPROPERTY(EditDefaultsOnly, Category = "LootChestItem")
	TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>> GrantedInputAbility;
};
