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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
};
