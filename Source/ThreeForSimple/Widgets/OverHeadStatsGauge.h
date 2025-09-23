// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadStatsGauge.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UOverHeadStatsGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureWithASC(class UAbilitySystemComponent* ASC);

private:
	UPROPERTY(meta = (BindWidget))
	class UValueGauge* HealthBar;
};
