// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/GAS/TfsGameplayAbility.h"
#include "GA_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_Weapon : public UTfsGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Weapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	virtual void StartShooting(FGameplayEventData PayLoad);
	UFUNCTION()
	virtual void StopShooting(FGameplayEventData PayLoad);

	UFUNCTION()
	virtual void Shoot(FGameplayEventData PayLoad);

	UFUNCTION()
	virtual void ShootAnimEnd(FGameplayEventData PayLoad);

	static FGameplayTag GetWeaponTag();

	bool bBasicAttackPressed = false;
	
	//Config
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float ShootRange = 3000.f;
	
	//Animations
	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	const TSubclassOf<UAnimInstance> AimAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	UAnimMontage* ShootMontage;

	void StartResetCanShootTimer(const bool bIsStopShooting = false);
	//Delay between shots
	void ResetCanShoot();

	bool bIsCircleLoadSuccessful;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float DelayBetweenShotsSeconds = 0.f;
	bool bCanShoot = true;
	


	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float CheckIfCanShootSeconds = 0.1f;
	FTimerHandle CheckIfCanShootTimerHandle;

	void TryShootAgain();

	UPROPERTY()
	class ATfsPlayerController* OwningTfsPlayerController;
	/***************************************************/
	/*			   Find if aiming at target			    /
	/***************************************************/
	AActor* GetAimTargetIfValid() const;

	void FindAimTarget();
	
	void StartAimTargetCheckTimer();
	void StopAimTargetCheckTimer();

	bool HasValidTarget() const;
	bool IsTargetInRange() const;

	void TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY()
	AActor* CurrentAimTarget;
	UPROPERTY()
	AActor* NewAimTarget;
	
	UPROPERTY()
	UAbilitySystemComponent* AimTargetAbilitySystemComponent;
	
	FTimerHandle AimTargetCheckTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	float AimTargetCheckTimeInterval = 0.01f;
};
