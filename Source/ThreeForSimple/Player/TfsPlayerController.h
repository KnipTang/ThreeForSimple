// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TfsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ATfsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// only called on the SERVER (Design of unreal)
	virtual void OnPossess(APawn* NewPawn) override;
	// only called on the CLIENT (Design of unreal), also on the listening server (P2P server setup)
	virtual void AcknowledgePossession(APawn* NewPawn) override;

private:
	UPROPERTY()
	class ATfsPlayerCharacter* TfsPlayerCharacter;
};
