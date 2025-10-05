// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/GAS/TfsGameplayAbility.h"
#include "GA_Interact.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_Interact : public UTfsGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Interact();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
    void Interact();
    void FindInteractableTarget();

	void SetupWaitInputPress();
	UFUNCTION()
	void HandleInputPress(float TimeWaited);

	void TryToInteract();
	
	class IInteractable* InteractableTarget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float InteractRange = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float FindInteractableTargetsInterval = 0.1f;
	
	FTimerHandle FindInteractableTargetsTimerHandle;
};
