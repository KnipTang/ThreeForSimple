// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

UTfsGameplayAbility::UTfsGameplayAbility()
{
}

UAnimInstance* UTfsGameplayAbility::GetAnimationInstance() const
{
	USkeletalMeshComponent* SkeletalMeshComponent = GetOwningComponentFromActorInfo();
	if (!SkeletalMeshComponent)
		return nullptr;

	return SkeletalMeshComponent->GetAnimInstance();
}

TArray<FHitResult> UTfsGameplayAbility::GetHitResultFromSweepLocationTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, ETeamAttitude::Type TargetTeam, bool bDrawDebug,
	bool bIgnoreSelf) const
{
	TArray<FHitResult> OutResult;
	TSet<AActor*> HitActors;

	IGenericTeamAgentInterface* OwnerGenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());

	for (const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
	{
		FVector StartLoc = TargetData->GetOrigin().GetTranslation();
		FVector EndLoc = TargetData->GetEndPoint();

		//For what type of objects to trace
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreSelf)
		{
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		}

		EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, SphereSweepRadius, ObjectTypes, false, ActorsToIgnore, DrawDebugTrace, HitResults, false);

		for (const FHitResult& Result : HitResults)
		{
			if (HitActors.Contains(Result.GetActor()))
			{
				continue;
			}

			if (OwnerGenericTeamAgentInterface)
			{
				if (OwnerGenericTeamAgentInterface->GetTeamAttitudeTowards(*Result.GetActor()) != TargetTeam)
				{
					continue;
				}
			}

			HitActors.Add(Result.GetActor());
			OutResult.Add(Result);
		}
	}

	return OutResult;
}

void UTfsGameplayAbility::PlayMontageLocally(UAnimMontage* MontageToPlay)
{
	UAnimInstance* OwnerAnimInst = GetAnimationInstance();
	if (OwnerAnimInst && !OwnerAnimInst->Montage_IsPlaying(MontageToPlay))
	{
		OwnerAnimInst->Montage_Play(MontageToPlay);
	}
}

AActor* UTfsGameplayAbility::GetAimTarget(float AimDistance, ETeamAttitude::Type TeamAttitude) const
{
	if (AActor* OwnerAvatarActor = GetAvatarActorFromActorInfo())
	{
		FVector Location;
		FRotator Rotation;
		OwnerAvatarActor->GetActorEyesViewPoint(Location, Rotation);

		FVector AimEnd = Location + Rotation.Vector() * AimDistance;

		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(OwnerAvatarActor);

		if (ShouldDrawDebug())
		{
			DrawDebugLine(GetWorld(), Location, AimEnd, FColor::Red, false, 2.f, 0U, 3.f);
		}
		
		TArray<FHitResult> HitResults;
		if (GetWorld()->LineTraceMultiByObjectType(HitResults, Location, AimEnd, CollisionObjectQueryParams, CollisionQueryParams))
		{
			for (FHitResult HitResult : HitResults)
				if (IsActorTeamAttitudeIs(HitResult.GetActor(), TeamAttitude))
					return HitResult.GetActor();
		}
	}

	return nullptr;
}

bool UTfsGameplayAbility::IsActorTeamAttitudeIs(const AActor* OtherActor, ETeamAttitude::Type TeamAttitude) const
{
	if (!OtherActor)
		return false;
	
	if (IGenericTeamAgentInterface* OwnerTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo()))
	{
		return OwnerTeamAgentInterface->GetTeamAttitudeTowards(*OtherActor) == TeamAttitude;
	}

	return false;
}

void UTfsGameplayAbility::SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData)
{
	if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo())
		OwnerASC->HandleGameplayEvent(EventTag, &EventData);
}

FGenericTeamId UTfsGameplayAbility::GetOwnerTeamId() const
{
	if (IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo()))
	{
		return OwnerTeamInterface->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

ACharacter* UTfsGameplayAbility::GetOwningCharacter()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	}

	return OwnerCharacter;
}

