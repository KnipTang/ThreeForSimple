// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadStatsGauge.h"

#include "ValueGauge.h"
#include "ThreeForSimple/GAS/TfsAttributeSet.h"

void UOverHeadStatsGauge::ConfigureWithASC(class UAbilitySystemComponent* ASC)
{
	if (!ASC)
		return;

	HealthBar->SetAndBoundToGameplayAttribute(ASC, UTfsAttributeSet::GetHealthAttribute(), UTfsAttributeSet::GetMaxHealthAttribute());
}
