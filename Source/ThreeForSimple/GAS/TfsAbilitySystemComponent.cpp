// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAbilitySystemComponent.h"
#include "TfsAttributeSet.h"

UTfsAbilitySystemComponent::UTfsAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UTfsAttributeSet::GetHealthAttribute()).AddUObject(this, &UTfsAbilitySystemComponent::HealthUpdated);
}

void UTfsAbilitySystemComponent::AuthApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& GameplayEffect, int Level)
{
	if(GetOwner() && GetOwner()->HasAuthority() && GameplayEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffect, Level, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UTfsAbilitySystemComponent::ApplyInitialEffects()
{
	if(!GetOwner() || !GetOwner()->HasAuthority())
		return;
	
	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialEffects)
	{
		if (EffectClass == nullptr)
			continue;
		AuthApplyGameplayEffect(EffectClass);
	}
}

void UTfsAbilitySystemComponent::GiveInitialAbilities()
{
	if(!GetOwner() || !GetOwner()->HasAuthority())
		return;
	
	for (const TPair<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPar : AddedAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPar.Value, 0, static_cast<int32>(AbilityPar.Key), nullptr));
	}
	for (const TPair<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPar : BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPar.Value, 1, static_cast<int32>(AbilityPar.Key), nullptr));
	}
}

void UTfsAbilitySystemComponent::ApplyFullStatEffect()
{
	AuthApplyGameplayEffect(FullStatEffect);
}

void UTfsAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!GetOwner())
		return;
	
	if (ChangeData.NewValue <= 0 && GetOwner()->HasAuthority() && DeathEffect)
	{
		AuthApplyGameplayEffect(DeathEffect);
	}
}
