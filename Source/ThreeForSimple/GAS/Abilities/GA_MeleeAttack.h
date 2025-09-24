// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/GAS/TfsGameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_MeleeAttack : public UTfsGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_MeleeAttack();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void SetupWaitComboInputPress();
	UFUNCTION()
	void HandleInputPress(float TimeWaited);
	void TryCommitNextCombo();
	
	//Damage
	UFUNCTION()
	void DoDamage(FGameplayEventData Data);
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DamageGameplayEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Targetting")
	float TargetSweepSphereRadius = 30.f;

	//Combo
	UFUNCTION()
	void MeleeAttackComboEventReceived(FGameplayEventData Data);
	
	static FGameplayTag GetMeleeAttackTargetEventTag();
	static FGameplayTag GetMeleeAttackComboEventTag();
	static FGameplayTag GetMeleeAttackComboEndEventTag();

	FName NextComboName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MeleeAttackMontage;
};
