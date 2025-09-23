// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
};
