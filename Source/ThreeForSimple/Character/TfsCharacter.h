// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TfsCharacter.generated.h"

UCLASS()
class THREEFORSIMPLE_API ATfsCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATfsCharacter();
	void ServerSideInit();
	void ClientSideInit();

	//Check if it's controlled by the owning player(locally) not another player or server or AI
	bool IsLocallyControlledByPlayer() const;

	//It only gets called on the server
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//***********************************************************//
	//					Gameplay Ability system
	//***********************************************************//
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "GAS")
	class UTfsAbilitySystemComponent* FtsAbilitySystemComponent;
	UPROPERTY()
	class UTfsAttributeSet* FtsAttributeSet;

	//***********************************************************//
	//							UI
	//***********************************************************//
private:
	void ConfigureOverHeadStatusWidget();
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* OverHeadWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;
	
	FTimerHandle HeadStatGaugeVisibilityUpdateTimerHandle;

	void UpdateHeadGaugeVisibility();
};
