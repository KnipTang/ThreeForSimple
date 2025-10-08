// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TfsGameplayAbilityTypes.h"
#include "TfsAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTfsAbilitySystemComponent();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//Lyra starter game inspired
	void TryActivateAbilitiesOnSpawn();
	
	void ApplyInitialEffects();
	void GiveInitialAbilities();

	void ApplyFullStatEffect();
	
	void AddInputAbility(ECAbilityInputID InputID, const TSubclassOf<UGameplayAbility>& GameplayAbility);
	void RemoveInputAbility(ECAbilityInputID InputID);
private:
	void AuthApplyGameplayEffect(const TSubclassOf<UGameplayEffect>& GameplayEffect, int Level = 1);

	void HealthUpdated(const FOnAttributeChangeData& ChangeData);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> FullStatEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DeathEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	//Abilities that you already have from the beginning of the game
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> OnSpawnBasicAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> InputBasicAbilities;
	
	//Abilities you learn during game
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> AddedAbilities;
};
