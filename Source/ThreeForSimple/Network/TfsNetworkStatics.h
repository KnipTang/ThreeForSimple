// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TfsNetworkStatics.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsNetworkStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static uint8 GetMaxPlayerCountPerTeam();
};
