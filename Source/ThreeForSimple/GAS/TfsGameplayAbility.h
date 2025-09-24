// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TfsGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAnimInstance* GetAnimationInstance();
	
	TArray<FHitResult> GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius,
		bool bDrawDebug, bool bIgnoreSelf) const;
};
