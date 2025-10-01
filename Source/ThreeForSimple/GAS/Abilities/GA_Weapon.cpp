// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Weapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ThreeForSimple/Character/TfsCharacter.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

UGA_Weapon::UGA_Weapon()
{
	ActivationOwnedTags.AddTag(UTfsAbilitySystemStatics::GetAimStatTag());
	ActivationOwnedTags.AddTag(UTfsAbilitySystemStatics::GetCrosshairStatTag());
}

void UGA_Weapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Hitscan ability activated"));

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		FindAimTarget();
		StartAimTargetCheckTimer();
		
		//Start hitscan
		UAbilityTask_WaitGameplayEvent* WaitStartShootingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UTfsAbilitySystemStatics::GetBasicAttackInputPressedTag());
		WaitStartShootingEvent->EventReceived.AddDynamic(this, &UGA_Weapon::StartShooting);
		WaitStartShootingEvent->ReadyForActivation();

		//Stop hitscan
		UAbilityTask_WaitGameplayEvent* WaitStopShootingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UTfsAbilitySystemStatics::GetBasicAttackInputReleasedTag());
		WaitStopShootingEvent->EventReceived.AddDynamic(this, &UGA_Weapon::StopShooting);
		WaitStopShootingEvent->ReadyForActivation();

		//Shooting hitscan
		UAbilityTask_WaitGameplayEvent* WaitShootProjectileEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,  GetWeaponTag(), nullptr, false, false);
		WaitShootProjectileEvent->EventReceived.AddDynamic(this, &UGA_Weapon::Shoot);
		WaitShootProjectileEvent->ReadyForActivation();
	}

	if (ATfsCharacter* OwningCharacter = Cast<ATfsCharacter>(GetOwningCharacter()))
		OwningCharacter->SetAnimInstance(AimAnimInstance);
}

void UGA_Weapon::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Warning, TEXT("Shoot ability ended"));
	K2_EndAbility();
}

void UGA_Weapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

void UGA_Weapon::StartShooting(FGameplayEventData PayLoad)
{
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

void UGA_Weapon::StopShooting(FGameplayEventData PayLoad)
{
	if (UAnimInstance* OwnerAnimInst = GetAnimationInstance())
	{
		FName CurrentSectionName = OwnerAnimInst->Montage_GetCurrentSection(ShootMontage);
		OwnerAnimInst->Montage_SetNextSection(CurrentSectionName, NAME_None, ShootMontage);
	}
}

void UGA_Weapon::Shoot(FGameplayEventData PayLoad)
{
}

FGameplayTag UGA_Weapon::GetWeaponTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Weapon");
}

AActor* UGA_Weapon::GetAimTargetIfValid() const
{
	if (HasValidTarget())
		return CurrentAimTarget;

	return nullptr;
}


void UGA_Weapon::FindAimTarget()
{
	NewAimTarget = GetAimTarget(ShootRange, ETeamAttitude::Hostile);
	
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
			AimTargetAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetDeadStatTag()).AddUObject(this, &UGA_Weapon::TargetDeadTagUpdated);
	}

	FGameplayEventData EventData;
	EventData.Target = CurrentAimTarget;
	SendLocalGameplayEvent(UTfsAbilitySystemStatics::GetTargetUpdatedTag(), EventData);
}

void UGA_Weapon::StartAimTargetCheckTimer()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().SetTimer(AimTargetCheckTimerHandle, this, &UGA_Weapon::FindAimTarget,AimTargetCheckTimeInterval,  true);
}

void UGA_Weapon::StopAimTargetCheckTimer()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().ClearTimer(AimTargetCheckTimerHandle);
}

bool UGA_Weapon::HasValidTarget() const
{
	if (!NewAimTarget)
		return false;

	if (UTfsAbilitySystemStatics::IsActorDead(NewAimTarget))
		return false;

	if (!IsTargetInRange())
		return false;

	return true;
}

bool UGA_Weapon::IsTargetInRange() const
{
	if (!CurrentAimTarget)
		return false;

	float Distance = FVector::Distance(CurrentAimTarget->GetActorLocation(), GetAvatarActorFromActorInfo()->GetActorLocation());
	return Distance <= ShootRange;
}

void UGA_Weapon::TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
		FindAimTarget();
}

