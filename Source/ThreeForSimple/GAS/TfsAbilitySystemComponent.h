// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TfsAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void ApplyInitialEffects();
	void GiveInitialAbilities();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	//Abilities that you already have from the beginning of the game
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UGameplayAbility>> BasicAbilities;
	
	//Abilities you learn during game
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UGameplayAbility>> AddedAbilities;
};
