// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"
#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

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

	if (!MeleeAttackMontage)
		return;
	
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

		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetMeleeAttackComboEventTag(), nullptr, false, false);
		WaitTargetingEventTask->EventReceived.AddDynamic(this, &UGA_MeleeAttack::MeleeAttackComboEventReceived);
		WaitTargetingEventTask->ReadyForActivation();
	}

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GetMeleeAttackTargetEventTag());
		WaitTargetingEventTask->EventReceived.AddDynamic(this, &UGA_MeleeAttack::DoDamage);
		WaitTargetingEventTask->ReadyForActivation();
	}

	SetupWaitComboInputPress();
}

void UGA_MeleeAttack::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPressTask->OnPress.AddDynamic(this, &UGA_MeleeAttack::HandleInputPress);
	WaitInputPressTask->ReadyForActivation();
}

void UGA_MeleeAttack::HandleInputPress(float TimeWaited)
{
	SetupWaitComboInputPress();
	TryCommitNextCombo();
}

void UGA_MeleeAttack::TryCommitNextCombo() const
{
	if (NextComboName == NAME_None)
		return;

	UAnimInstance* AnimInstance = GetAnimationInstance();
	if (!AnimInstance)
		return;

	AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(MeleeAttackMontage), NextComboName, MeleeAttackMontage);
}

void UGA_MeleeAttack::DoDamage(FGameplayEventData Data)
{
	TArray<FHitResult> HitResults = GetHitResultFromSweepLocationTargetData(Data.TargetData, TargetSweepSphereRadius);

	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	
	for (const FHitResult& Result : HitResults)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GetDamageEffectForCurrentCombo(), GetAbilityLevel(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo()));

		FGameplayAbilitySpecHandle abilitySpecHandle = GetCurrentAbilitySpecHandle();
		const FGameplayAbilityActorInfo* AbilityActorInfo = GetCurrentActorInfo();
		FGameplayEffectContextHandle EffectContextHandle =  MakeEffectContext(abilitySpecHandle, AbilityActorInfo);
		EffectContextHandle.AddHitResult(Result);
		
		EffectSpecHandle.Data->SetContext(EffectContextHandle);
		
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Result.GetActor()));
	}
}

TSubclassOf<UGameplayEffect> UGA_MeleeAttack::GetDamageEffectForCurrentCombo() const
{
	const UAnimInstance* AnimInstance = GetAnimationInstance();
	if (!AnimInstance)
		return nullptr;

	const FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(MeleeAttackMontage);
	if (const TSubclassOf<UGameplayEffect>* FoundEffectPtr = DamageGameplayEffectsMap.Find(CurrentSectionName))
		return *FoundEffectPtr;

	return nullptr;
}

void UGA_MeleeAttack::MeleeAttackComboEventReceived(FGameplayEventData Data)
{
	const FGameplayTag EventTag = Data.EventTag;

	if (EventTag == GetMeleeAttackComboEndEventTag())
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("next combo is cleared"));
		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();

	UE_LOG(LogTemp, Warning, TEXT("next combo is now: %s"), *NextComboName.ToString());
}

FGameplayTag UGA_MeleeAttack::GetMeleeAttackTargetEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Damage");
}

FGameplayTag UGA_MeleeAttack::GetMeleeAttackComboEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Combo");
}

FGameplayTag UGA_MeleeAttack::GetMeleeAttackComboEndEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.MeleeAttack.Combo.End");
}
