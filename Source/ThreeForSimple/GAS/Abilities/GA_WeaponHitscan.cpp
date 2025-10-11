// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponHitscan.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

void UGA_WeaponHitscan::Shoot(FGameplayEventData PayLoad)
{
	Super::Shoot(PayLoad);
	
	AActor* OwningActor = GetAvatarActorFromActorInfo();

	if (!CurrentAimTarget || CurrentAimTarget == OwningActor)
		return;

	if (!IsActorTeamAttitudeIs(CurrentAimTarget, ETeamAttitude::Hostile))
		return;
	
	UTfsAbilitySystemStatics::ApplyEffect(OwningActor, CurrentAimTarget, MakeOutgoingGameplayEffectSpec(HitscanHitEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo)));
	
	FGameplayCueParameters CueParams;
	CueParams.Location = CurrentAimTarget->GetActorLocation();
	CueParams.Normal = CurrentAimTarget->GetActorForwardVector();
	AimTargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentAimTarget);
	AimTargetAbilitySystemComponent->ExecuteGameplayCue(HitGameplayCueTag, CueParams);
}
