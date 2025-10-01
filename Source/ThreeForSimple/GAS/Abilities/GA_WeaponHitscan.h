// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Weapon.h"
#include "GA_WeaponHitscan.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UGA_WeaponHitscan : public UGA_Weapon
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot(FGameplayEventData PayLoad) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Cue")
	FGameplayTag HitGameplayCueTag;
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	TSubclassOf<UGameplayEffect> HitscanHitEffect;
};
