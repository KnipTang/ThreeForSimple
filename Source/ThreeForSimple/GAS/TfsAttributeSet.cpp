// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UTfsAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTfsAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTfsAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UTfsAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
}

void UTfsAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
}

void UTfsAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	//Tells ASC that property as changed on server, This will trigger to replicate it towards to client
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTfsAttributeSet, Health, OldValue);
}

void UTfsAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	//Tells ASC that property as changed on server, This will trigger to replicate it towards to client
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTfsAttributeSet, MaxHealth, OldValue);
}
