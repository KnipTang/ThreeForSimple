// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/GAS/TfsGameplayAbility.h"
#include "GA_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_Shoot : public UTfsGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Shoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<class AProjectileActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<UGameplayEffect> ProjectileHitEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float ShootProjectileSpeed = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float ShootProjectileRange = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	FName ShootSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	const TSubclassOf<UAnimInstance> AimnAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* ShootMontage;
	
	UFUNCTION()
	void StartShooting(FGameplayEventData PayLoad);
	UFUNCTION()
	void StopShooting(FGameplayEventData PayLoad);

	UFUNCTION()
	void ShootProjectile(FGameplayEventData PayLoad);
	AActor* GetAimTargetIfValid() const;

	static FGameplayTag GetShootTag();

	/***************************************************/
	/*			   Find if aiming at target			    /
	/***************************************************/
	UPROPERTY()
	AActor* CurrentAimTarget;
	UPROPERTY()
	AActor* NewAimTarget;
	
	UPROPERTY()
	UAbilitySystemComponent* AimTargetAbilitySystemComponent;
	
	FTimerHandle AimTargetCheckTimerHandle;
	
	void FindAimTarget();
	
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	float AimTargetCheckTimeInterval = 0.01f;
	
	void StartAimTargetCheckTimer();
	void StopAimTargetCheckTimer();

	bool HasValidTarget() const;
	bool IsTargetInRange() const;

	void TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount);
};
