// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponHitscan.h"
#include "GenericTeamAgentInterface.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

void UGA_WeaponHitscan::Shoot(FGameplayEventData PayLoad)
{
	Super::Shoot(PayLoad);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Hitscan"));

	if (K2_HasAuthority())
	{
		AActor* OwningActor = GetAvatarActorFromActorInfo();
	
		if (!CurrentAimTarget || CurrentAimTarget == OwningActor)
			return;
	
		if (!IsActorTeamAttitudeIs(CurrentAimTarget, ETeamAttitude::Hostile))
			return;
	
		UTfsAbilitySystemStatics::ApplyEffect(OwningActor, CurrentAimTarget, MakeOutgoingGameplayEffectSpec(HitscanHitEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo)));

		FHitResult HitResult;
		HitResult.ImpactPoint = CurrentAimTarget->GetActorLocation();
		HitResult.ImpactNormal = CurrentAimTarget->GetActorForwardVector();
		UTfsAbilitySystemStatics::SendLocalGameplayCue(CurrentAimTarget, HitResult, HitGameplayCueTag);
	}
}
