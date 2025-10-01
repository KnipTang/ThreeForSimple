// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "TfsGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UAnimInstance* GetAnimationInstance() const;
	
	TArray<FHitResult> GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius = 30.f,
		ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile, bool bDrawDebug = false, bool bIgnoreSelf = true) const;

	void PlayMontageLocally(UAnimMontage* MontageToPlay);
	AActor* GetAimTarget(float AimDistance, ETeamAttitude::Type TeamAttitude) const;

	bool IsActorTeamAttitudeIs(const AActor* OtherActor, ETeamAttitude::Type TeamAttitude) const;

	void SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData);

	FGenericTeamId GetOwnerTeamId() const;
	
	UFUNCTION()
	FORCEINLINE bool ShouldDrawDebug() const { return bShouldDrawDebug; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShouldDrawDebug = false;
};
