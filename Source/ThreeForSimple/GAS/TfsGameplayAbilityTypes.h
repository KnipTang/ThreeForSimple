// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TfsGameplayAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class ECAbilityInputID : uint8
{
	None							UMETA(DisplayName = "None"),
	BasicAttack						UMETA(DisplayName = "Melee Attack"),
	Aim								UMETA(DisplayName = "Aim"),
	Interact						UMETA(DisplayName = "Interact"),
	Confirm							UMETA(DisplayName = "Confirm"),
	Cancel							UMETA(DisplayName = "Cancel"),
};
