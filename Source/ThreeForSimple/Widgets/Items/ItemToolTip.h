// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTip.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UItemToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItem(const class UPA_LootChestItem* Item);
private:
	UPROPERTY(meta=(bindWidget))
	class UImage* ItemIcon;
	
	UPROPERTY(meta=(bindWidget))
	class UTextBlock* ItemTitleText;
	
	UPROPERTY(meta=(bindWidget))
	class UTextBlock* ItemDescriptionText;
};
