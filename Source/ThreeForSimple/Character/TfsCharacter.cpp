// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"
#include "ThreeForSimple/GAS/TfsAttributeSet.h"
#include "ThreeForSimple/Widgets/OverHeadStatsGauge.h"

// Sets default values
ATfsCharacter::ATfsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TfsAbilitySystemComponent = CreateDefaultSubobject<UTfsAbilitySystemComponent>("Ability System Component");
	TfsAttributeSet = CreateDefaultSubobject<UTfsAttributeSet>("Attribute Set");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());

	BindGASChangeDelegate();
}

void ATfsCharacter::ServerSideInit()
{
	TfsAbilitySystemComponent->InitAbilityActorInfo(this, this);
	TfsAbilitySystemComponent->ApplyInitialEffects();
	TfsAbilitySystemComponent->GiveInitialAbilities();
}

void ATfsCharacter::ClientSideInit()
{
	TfsAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

bool ATfsCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ATfsCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Check if its AI Controller
	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

// Called when the game starts or when spawned
void ATfsCharacter::BeginPlay()
{
	Super::BeginPlay();

	ConfigureOverHeadStatusWidget();
	MeshRelativeTransform = GetMesh()->GetRelativeTransform();

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		DefaultAnimInstance = MeshComp->GetAnimClass();
	}
}

// Called every frame
void ATfsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATfsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATfsCharacter::SetAnimInstance(const TSubclassOf<UAnimInstance>& AnimInstance)
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetAnimInstanceClass(AnimInstance);
	}
}

void ATfsCharacter::ResetAnimInstanceToDefault()
{
	SetAnimInstance(DefaultAnimInstance);
}

UAbilitySystemComponent* ATfsCharacter::GetAbilitySystemComponent() const
{
	return TfsAbilitySystemComponent;
}

void ATfsCharacter::Server_SendGameplayEventToSelf_Implementation(const FGameplayTag& EventTag, const FGameplayEventData& EventData)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, EventData);
}

bool ATfsCharacter::Server_SendGameplayEventToSelf_Validate(const FGameplayTag& EventTag, const FGameplayEventData& EventData)
{
	return true;
}

void ATfsCharacter::BindGASChangeDelegate()
{
	if (TfsAbilitySystemComponent)
	{
		TfsAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetDeadStatTag()).AddUObject(this, &ATfsCharacter::DeathTagUpdated);
		TfsAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetAimStatTag()).AddUObject(this, &ATfsCharacter::AimTagUpdated);
	}
}

void ATfsCharacter::DeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
		StartDeathSequence();
	else
		StartRespawnSequence();
}

void ATfsCharacter::ConfigureOverHeadStatusWidget()
{
	if (!OverHeadWidgetComponent)
		return;

	if (IsLocallyControlledByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}
	
	if (UOverHeadStatsGauge* OverHeadStatsGauge = Cast<UOverHeadStatsGauge>(OverHeadWidgetComponent->GetUserWidgetObject()))
	{
		OverHeadStatsGauge->ConfigureWithASC(GetAbilitySystemComponent());
		OverHeadWidgetComponent->SetHiddenInGame(false);
		
		GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(HeadStatGaugeVisibilityUpdateTimerHandle, this, &ATfsCharacter::UpdateHeadGaugeVisibility, HeadStatGaugeVisibilityCheckUpdateGap, true);
	}
}

void ATfsCharacter::UpdateHeadGaugeVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverHeadWidgetComponent->SetHiddenInGame(DistSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}

void ATfsCharacter::SetStatusGaugeVisibility(bool bIsVisibility)
{
	GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);
	if (bIsVisibility)
	{
		ConfigureOverHeadStatusWidget();
	}
	else
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
	}
}

void ATfsCharacter::AimTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	SetIsAiming(NewCount != 0);
}

void ATfsCharacter::SetIsAiming(const bool bIsAiming)
{
	bUseControllerRotationYaw = bIsAiming;
	GetCharacterMovement()->bOrientRotationToMovement = !bIsAiming;
	OnAimStateChanged(bIsAiming);
}

void ATfsCharacter::StartDeathSequence()
{
	OnDead();
	PlayDeathMontage();
	SetStatusGaugeVisibility(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATfsCharacter::StartRespawnSequence()
{
	OnRespawn();
	SetRagdollEnabled(false);
	SetStatusGaugeVisibility(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);

	if (TfsAbilitySystemComponent)
		TfsAbilitySystemComponent->ApplyFullStatEffect();
}

void ATfsCharacter::PlayDeathMontage()
{
	if (DeathMontage)
	{
		float MontageDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(DeathMontageTimerHandle, this, &ATfsCharacter::DeathMontageFinished, MontageDuration + DeathMontageFinishTimeShift);
	}
}

void ATfsCharacter::DeathMontageFinished() const
{
	SetRagdollEnabled(true);
}

void ATfsCharacter::SetRagdollEnabled(bool bIsEnabled) const
{
	if (bIsEnabled)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshRelativeTransform);
	}
}

void ATfsCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ATfsCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ATfsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATfsCharacter, TeamID);
}

