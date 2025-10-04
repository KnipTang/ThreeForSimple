// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Interact.h"

#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

UGA_Interact::UGA_Interact()
{
	AbilityTags.AddTag(UTfsAbilitySystemStatics::GetMeleeAttackAbilityTag());
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	//if (GetAimTarget(InteractRange, ))
}
