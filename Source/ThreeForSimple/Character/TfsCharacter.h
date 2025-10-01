// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "TfsCharacter.generated.h"

UCLASS()
class THREEFORSIMPLE_API ATfsCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
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

	void SetAnimInstance(const TSubclassOf<UAnimInstance>& AnimInstance);
	void ResetAnimInstanceToDefault();
	
private:
	UPROPERTY()
	UClass* DefaultAnimInstance;
	//***********************************************************//
	//					Gameplay Ability system
	//***********************************************************//
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendGameplayEventToSelf(const FGameplayTag& EventTag, const FGameplayEventData& EventData);
private:
	void BindGASChangeDelegate();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(VisibleDefaultsOnly, Category = "GAS")
	class UTfsAbilitySystemComponent* TfsAbilitySystemComponent;
	UPROPERTY()
	class UTfsAttributeSet* TfsAttributeSet;

	//***********************************************************//
	//							 UI								 //
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
	void SetStatusGaugeVisibility(bool bIsVisibility);
	
	//***********************************************************//
	//							Weapon							 //
	//***********************************************************//
public:
	void AimTagUpdated(const FGameplayTag Tag, int32 NewCount);
	void SetIsAiming(const bool bIsAiming);
	virtual void OnAimStateChanged(const bool bIsAiming) {};
	
	//***********************************************************//
    //						Death and Respawn					 //
    //***********************************************************//
private:
	FTransform MeshRelativeTransform;
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathMontageFinishTimeShift = -0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	FTimerHandle DeathMontageTimerHandle;
	
	void StartDeathSequence();
	void StartRespawnSequence();

	virtual void OnDead() {};
	virtual void OnRespawn() {};

	void PlayDeathMontage();
	void DeathMontageFinished() const;

	void SetRagdollEnabled(bool bIsEnabled) const;
	
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
