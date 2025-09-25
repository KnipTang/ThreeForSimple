// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsAbilitySystemStatics.h"

FGameplayTag UTfsAbilitySystemStatics::GetMeleeAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack");
}

FGameplayTag UTfsAbilitySystemStatics::GetDeadStatTag()
{
	return FGameplayTag::RequestGameplayTag("Stats.Dead");
}
