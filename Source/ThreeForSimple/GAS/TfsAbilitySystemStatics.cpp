// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAbilitySystemStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueManager.h"

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

void UTfsAbilitySystemStatics::ApplyEffect(AActor* OwnerActor, AActor* OtherActor, const FGameplayEffectSpecHandle& EffectSpecHandle)
{
	UAbilitySystemComponent* OtherASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (IsValid(OtherASC))
	{
		if (OwnerActor->HasAuthority() && EffectSpecHandle.IsValid())
		{
			OtherASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void UTfsAbilitySystemStatics::SendLocalGameplayCue(AActor* CueTargetActor, const FHitResult& HitResult, const FGameplayTag& HitGameplayCueTag)
{
	FGameplayCueParameters CueParams;
	CueParams.Location = HitResult.ImpactPoint;
	CueParams.Normal = HitResult.ImpactNormal;

	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(CueTargetActor, HitGameplayCueTag, EGameplayCueEvent::Executed, CueParams);
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

