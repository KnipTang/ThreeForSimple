// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Shoot.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectileActor.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ThreeForSimple/Character/TfsCharacter.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

UGA_Shoot::UGA_Shoot()
{
	ActivationOwnedTags.AddTag(UTfsAbilitySystemStatics::GetAimStatTag());
	ActivationOwnedTags.AddTag(UTfsAbilitySystemStatics::GetCrosshairStatTag());
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Shoot ability activated"));

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		FindAimTarget();
		StartAimTargetCheckTimer();
		
		//Start shooting
		UAbilityTask_WaitGameplayEvent* WaitStartShootingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UTfsAbilitySystemStatics::GetBasicAttackInputPressedTag());
		WaitStartShootingEvent->EventReceived.AddDynamic(this, &UGA_Shoot::StartShooting);
		WaitStartShootingEvent->ReadyForActivation();

		//Stop shooting
		UAbilityTask_WaitGameplayEvent* WaitStopShootingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UTfsAbilitySystemStatics::GetBasicAttackInputReleasedTag());
		WaitStopShootingEvent->EventReceived.AddDynamic(this, &UGA_Shoot::StopShooting);
		WaitStopShootingEvent->ReadyForActivation();

		//Shooting projectile
		UAbilityTask_WaitGameplayEvent* WaitShootProjectileEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,  GetShootTag(), nullptr, false, false);
		WaitShootProjectileEvent->EventReceived.AddDynamic(this, &UGA_Shoot::ShootProjectile);
		WaitShootProjectileEvent->ReadyForActivation();
	}

	if (ATfsCharacter* OwningCharacter = Cast<ATfsCharacter>(GetOwningCharacter()))
		OwningCharacter->SetAnimInstance(AimnAnimInstance);
}

void UGA_Shoot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Warning, TEXT("Shoot ability ended"));
	K2_EndAbility();
}

void UGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AimTargetAbilitySystemComponent)
	{
		AimTargetAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetDeadStatTag()).RemoveAll(this);
		AimTargetAbilitySystemComponent = nullptr;
	}
	
	if (ATfsCharacter* OwningCharacter = Cast<ATfsCharacter>(GetOwningCharacter()))
		OwningCharacter->ResetAnimInstanceToDefault();
	
	StopShooting(FGameplayEventData());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Shoot::StartShooting(FGameplayEventData PayLoad)
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot Start"));

	if (!ShootMontage)
		return;
	
	//Plays only on the server
	if (HasAuthority(&CurrentActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayShootMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ShootMontage);
		PlayShootMontage->ReadyForActivation();
	}
	else
	{
		PlayMontageLocally(ShootMontage);
	}
}

void UGA_Shoot::StopShooting(FGameplayEventData PayLoad)
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot Stop"));
	if (UAnimInstance* OwnerAnimInst = GetAnimationInstance())
	{
		FName CurrentSectionName = OwnerAnimInst->Montage_GetCurrentSection(ShootMontage);
		OwnerAnimInst->Montage_SetNextSection(CurrentSectionName, NAME_None, ShootMontage);
	}
}

void UGA_Shoot::ShootProjectile(FGameplayEventData PayLoad)
{
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
			Projectile->ShootProjectile(ShootProjectileSpeed, ShootProjectileRange, GetAimTargetIfValid(), GetOwnerTeamId(), MakeOutgoingGameplayEffectSpec(ProjectileHitEffect, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo)));
		}
	}
}

AActor* UGA_Shoot::GetAimTargetIfValid() const
{
	if (HasValidTarget())
		return CurrentAimTarget;

	return nullptr;
}

FGameplayTag UGA_Shoot::GetShootTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Shoot");
}

void UGA_Shoot::FindAimTarget()
{
	NewAimTarget = GetAimTarget(ShootProjectileRange, ETeamAttitude::Hostile);
	
	if (HasValidTarget())
		return;
	
	if (NewAimTarget == CurrentAimTarget)
	{
		return;
	}
	CurrentAimTarget = NewAimTarget;
	
	if (AimTargetAbilitySystemComponent)
	{
		AimTargetAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetDeadStatTag()).RemoveAll(this);
		AimTargetAbilitySystemComponent = nullptr;
	}
	
	if (CurrentAimTarget)
	{
		AimTargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentAimTarget);
		if (AimTargetAbilitySystemComponent)
			AimTargetAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetDeadStatTag()).AddUObject(this, &UGA_Shoot::TargetDeadTagUpdated);
	}

	FGameplayEventData EventData;
	EventData.Target = CurrentAimTarget;
	SendLocalGameplayEvent(UTfsAbilitySystemStatics::GetTargetUpdatedTag(), EventData);
}

void UGA_Shoot::StartAimTargetCheckTimer()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().SetTimer(AimTargetCheckTimerHandle, this, &UGA_Shoot::FindAimTarget,AimTargetCheckTimeInterval,  true);
}

void UGA_Shoot::StopAimTargetCheckTimer()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().ClearTimer(AimTargetCheckTimerHandle);
}

bool UGA_Shoot::HasValidTarget() const
{
	if (!NewAimTarget)
		return false;

	if (UTfsAbilitySystemStatics::IsActorDead(NewAimTarget))
		return false;

	if (!IsTargetInRange())
		return false;

	return true;
}

bool UGA_Shoot::IsTargetInRange() const
{
	if (!CurrentAimTarget)
		return false;

	float Distance = FVector::Distance(CurrentAimTarget->GetActorLocation(), GetAvatarActorFromActorInfo()->GetActorLocation());
	return Distance <= ShootProjectileRange;
}

void UGA_Shoot::TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
		FindAimTarget();
}
