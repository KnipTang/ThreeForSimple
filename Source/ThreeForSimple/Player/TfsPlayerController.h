// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "TfsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ATfsPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// only called on the SERVER (Design of unreal)
	virtual void OnPossess(APawn* NewPawn) override;
	// only called on the CLIENT (Design of unreal), also on the listening server (P2P server setup)
	virtual void AcknowledgePossession(APawn* NewPawn) override;
private:
	void SpawnGameplayWidget();
	
	UPROPERTY()
	class ATfsPlayerCharacter* TfsPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;

	//***********************************************************//
	//							Team							 //
	//***********************************************************//
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	FGenericTeamId TeamID;
};
