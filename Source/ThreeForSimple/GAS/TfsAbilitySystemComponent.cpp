// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAbilitySystemComponent.h"

void UTfsAbilitySystemComponent::ApplyInitialEffects()
{
	if(!GetOwner() || !GetOwner()->HasAuthority())
		return;
	
	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialEffects)
	{
		if (EffectClass == nullptr)
			continue;
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}
