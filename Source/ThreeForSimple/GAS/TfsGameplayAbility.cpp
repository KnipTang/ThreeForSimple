// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"

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
