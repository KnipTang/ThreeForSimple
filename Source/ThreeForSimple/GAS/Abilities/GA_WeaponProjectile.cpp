// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectileActor.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ThreeForSimple/Character/TfsCharacter.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

void UGA_WeaponProjectile::Shoot(FGameplayEventData PayLoad)
{
	Super::Shoot(PayLoad);

	UE_LOG(LogTemp, Warning, TEXT("Shoot Projectile"));
	if (K2_HasAuthority())
	{
		AActor* OwnerAvatarActor = GetAvatarActorFromActorInfo();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerAvatarActor;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector SocketLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		if (USkeletalMeshComponent* MeshComp = GetOwningComponentFromActorInfo())
			SocketLocation = MeshComp->GetSocketLocation(ShootSocketName);
		
		if (AProjectileActor* Projectile = GetWorld()->SpawnActor<AProjectileActor>(ProjectileClass, SocketLocation, OwnerAvatarActor->GetActorRotation(), SpawnParams))
		{
			Projectile->ShootProjectile(ShootProjectileSpeed, ShootRange, GetAimTargetIfValid(), GetOwnerTeamId(), MakeOutgoingGameplayEffectSpec(ProjectileHitEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo)));
		}
	}
}