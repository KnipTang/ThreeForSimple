// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Weapon.h"
#include "GA_WeaponProjectile.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_WeaponProjectile : public UGA_Weapon
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot(FGameplayEventData PayLoad) override;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<class AProjectileActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<UGameplayEffect> ProjectileHitEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	float ShootProjectileSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	FName ShootSocketName;
};
