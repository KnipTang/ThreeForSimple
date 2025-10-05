// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Interact.h"

#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"
#include "ThreeForSimple/Interactables/Interactable.h"

UGA_Interact::UGA_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (K2_HasAuthority())
	{
		SetupWaitInputPress();
	}

	if (UWorld* World = GetWorld())
		 World->GetTimerManager().SetTimer(FindInteractableTargetsTimerHandle, this, &UGA_Interact::FindInteractableTarget, FindInteractableTargetsInterval, true);
}

void UGA_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UE_LOG(LogTemp, Warning, TEXT("EndInteract"));

}

void UGA_Interact::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacting %s"), *InteractableTarget->GetName());
	InteractableTarget->OnInteract(GetAvatarActorFromActorInfo());

	FGameplayEventData EventData;
	EventData.Target = GetAvatarActorFromActorInfo();
	SendLocalGameplayEvent(UTfsAbilitySystemStatics::GetInteractActivateTag(), EventData);
}

void UGA_Interact::FindInteractableTarget()
{
	for (const FHitResult HitResult : GetHitscanTargets(InteractRange, ECC_WorldStatic))
	{
		AActor* PossibleInteractableActor = HitResult.GetActor();
		if (IInteractable* InteractableActorInterface = Cast<IInteractable>(PossibleInteractableActor))
		{
			InteractableTarget = InteractableActorInterface;
			return;
		}
	}
	InteractableTarget = nullptr;
}

void UGA_Interact::SetupWaitInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPressTask->OnPress.AddDynamic(this, &UGA_Interact::HandleInputPress);
	WaitInputPressTask->ReadyForActivation();
}

void UGA_Interact::HandleInputPress(float TimeWaited)
{
	UE_LOG(LogTemp, Warning, TEXT("InputInteract"));
	SetupWaitInputPress();
	TryToInteract();
}

void UGA_Interact::TryToInteract()
{
	if (!InteractableTarget)
		return;
	Interact();
}
