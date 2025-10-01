// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponHitscan.h"

void UGA_WeaponHitscan::Shoot(FGameplayEventData PayLoad)
{
	Super::Shoot(PayLoad);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Hitscan"));

	if (K2_HasAuthority())
	{
		//ImplementHitscan
	}
}
