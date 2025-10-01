// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAbilitySystemStatics.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

FGameplayTag UTfsAbilitySystemStatics::GetMeleeAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack");
}

FGameplayTag UTfsAbilitySystemStatics::GetBasicAttackInputPressedTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Pressed");
}

FGameplayTag UTfsAbilitySystemStatics::GetBasicAttackInputReleasedTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Released");
}

FGameplayTag UTfsAbilitySystemStatics::GetDeadStatTag()
{
	return FGameplayTag::RequestGameplayTag("Stats.Dead");
}

FGameplayTag UTfsAbilitySystemStatics::GetAimStatTag()
{
	return FGameplayTag::RequestGameplayTag("Stats.Aim");
}

FGameplayTag UTfsAbilitySystemStatics::GetCrosshairStatTag()
{
	return FGameplayTag::RequestGameplayTag("Stats.Crosshair");
}

FGameplayTag UTfsAbilitySystemStatics::GetTargetUpdatedTag()
{
	return FGameplayTag::RequestGameplayTag("Target.Updated");
}

bool UTfsAbilitySystemStatics::IsActorDead(const AActor* ActorToCheck)
{
	return ActorHasTag(ActorToCheck, GetDeadStatTag());
}

bool UTfsAbilitySystemStatics::ActorHasTag(const AActor* ActorToCheck, const FGameplayTag& Tag)
{
	if (const IAbilitySystemInterface* ActorISA = Cast<IAbilitySystemInterface>(ActorToCheck))
	{
		if (UAbilitySystemComponent* ActorASC = ActorISA->GetAbilitySystemComponent())
			return ActorASC->HasMatchingGameplayTag(Tag);
	}
	return false;
}

