// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

UGA_MeleeAttack::UGA_MeleeAttack()
{
	AbilityTags.AddTag(UTfsAbilitySystemStatics::GetMeleeAttackAbilityTag());
	BlockAbilitiesWithTag.AddTag(UTfsAbilitySystemStatics::GetMeleeAttackAbilityTag());
}

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Checks if you have for example enough points for the ability and the ability is not on cooldown
	if (!K2_CommitAbility())
	{
		//Stops the ability otherwise it goes on forever
		K2_EndAbility();
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Active Ability"));
	
	//Predication key: The one that send out the ability has the prediction key and can therefor already play the animation locally and correct it once if needed the server executed the animation and replicates the data back
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		//Plays on server but automatically replicates it to all clients
		UAbilityTask_PlayMontageAndWait* PlayMeleeAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MeleeAttackMontage);
		PlayMeleeAttackMontageTask->OnBlendOut.AddDynamic(this, &UGA_MeleeAttack::K2_EndAbility);
		PlayMeleeAttackMontageTask->OnCancelled.AddDynamic(this, &UGA_MeleeAttack::K2_EndAbility);
		PlayMeleeAttackMontageTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack::K2_EndAbility);
		PlayMeleeAttackMontageTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack::K2_EndAbility);
		PlayMeleeAttackMontageTask->ReadyForActivation();
	}

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetMeleeAttackTargetEventTag());
		WaitTargetingEventTask->EventReceived.AddDynamic(this, &UGA_MeleeAttack::DoDamage);
		WaitTargetingEventTask->ReadyForActivation();
	}
}

void UGA_MeleeAttack::DoDamage(FGameplayEventData Data)
{
	if (!DamageGameplayEffects)
		return;
	
	TArray<FHitResult> HitResults = GetHitResultFromSweepLocationTargetData(Data.TargetData, TargetSweepSphereRadius, true, true);

	for (const FHitResult& Result : HitResults)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffects, 1);

		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		EffectContextHandle.AddHitResult(Result);
		
		EffectSpecHandle.Data->SetContext(EffectContextHandle);
		
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Result.GetActor()));
	}
}

FGameplayTag UGA_MeleeAttack::GetMeleeAttackTargetEventTag() const
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Damage");
}
