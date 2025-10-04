// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TfsAbilitySystemStatics.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayTag GetMeleeAttackAbilityTag();
	static FGameplayTag GetBasicAttackInputPressedTag();
	static FGameplayTag GetBasicAttackInputReleasedTag();
	static FGameplayTag GetDeadStatTag();
	static FGameplayTag GetAimStatTag();
	static FGameplayTag GetCrosshairStatTag();
	static FGameplayTag GetTargetUpdatedTag();
	static FGameplayTag GetInteractTag();

	static void ApplyEffect(AActor* OwnerActor, AActor* OtherActor, const struct FGameplayEffectSpecHandle& EffectSpecHandle);
	static void SendLocalGameplayCue(AActor* CueTargetActor, const FHitResult& HitResult, const FGameplayTag& HitGameplayCueTag);
	
	static bool IsActorDead(const AActor* ActorToCheck);
	static bool ActorHasTag(const AActor* ActorToCheck, const FGameplayTag& Tag);
};
