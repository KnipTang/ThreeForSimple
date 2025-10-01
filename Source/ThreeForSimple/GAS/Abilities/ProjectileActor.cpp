// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Net/UnrealNetwork.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	bReplicates = true;
}

void AProjectileActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AActor* OwningActor = GetOwner();
	
	if (!OtherActor || OtherActor == OwningActor)
		return;
	
	if (GetTeamAttitudeTowards(*OtherActor) != TargetTeamAttitude)
		return;
	
	UTfsAbilitySystemStatics::ApplyEffect(OwningActor, OtherActor, HitEffectSpecHandle);

	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	HitResult.ImpactNormal = GetActorForwardVector();
	UTfsAbilitySystemStatics::SendLocalGameplayCue(OtherActor, HitResult, HitGameplayCueTag);
	
	if (HasAuthority())
	{
		GetWorldTimerManager().ClearTimer(ShootTimerHandle);
		Destroy();
	}
}

void AProjectileActor::ShootProjectile(float InSpeed, float InMaxDistance, const AActor* InTarget,
	FGenericTeamId InTeamId, FGameplayEffectSpecHandle InHitEffectHandle, ETeamAttitude::Type InTargetTeamAttitude)
{
	Target = InTarget;
	ProjectileSpeed = InSpeed;
	SetGenericTeamId(InTeamId);

	FRotator OwnerViewRot = GetActorRotation();

	if (GetOwner())
	{
		FVector OwnerViewLoc;
		GetOwner()->GetActorEyesViewPoint(OwnerViewLoc, OwnerViewRot);
	}

	MoveDir = OwnerViewRot.Vector();

	HitEffectSpecHandle = InHitEffectHandle;
	TargetTeamAttitude = static_cast<int>(InTargetTeamAttitude);

	if (HasAuthority())
	{
		if (Target)
		{
			MoveDir = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		}
	}

	float TravelMaxTime = InMaxDistance / InSpeed;
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AProjectileActor::TravelMaxDistanceReached, TravelMaxTime);
}

void AProjectileActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileActor, MoveDir);
	DOREPLIFETIME(AProjectileActor, TeamId);
	DOREPLIFETIME(AProjectileActor, ProjectileSpeed);
	DOREPLIFETIME(AProjectileActor, TargetTeamAttitude);
}

void AProjectileActor::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

FGenericTeamId AProjectileActor::GetGenericTeamId() const
{
	return TeamId;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + MoveDir * ProjectileSpeed * DeltaTime);
}

void AProjectileActor::TravelMaxDistanceReached()
{
	Destroy();
}

