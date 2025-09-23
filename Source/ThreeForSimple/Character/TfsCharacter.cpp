// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsCharacter.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"
#include "ThreeForSimple/GAS/TfsAttributeSet.h"
#include "ThreeForSimple/Widgets/OverHeadStatsGauge.h"

// Sets default values
ATfsCharacter::ATfsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FtsAbilitySystemComponent = CreateDefaultSubobject<UTfsAbilitySystemComponent>("Ability System Component");
	FtsAttributeSet = CreateDefaultSubobject<UTfsAttributeSet>("Attribute Set");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
}

void ATfsCharacter::ServerSideInit()
{
	FtsAbilitySystemComponent->InitAbilityActorInfo(this, this);
	FtsAbilitySystemComponent->ApplyInitialEffects();
	FtsAbilitySystemComponent->GiveInitialAbilities();
}

void ATfsCharacter::ClientSideInit()
{
	FtsAbilitySystemComponent->InitAbilityActorInfo(this, this);
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

UAbilitySystemComponent* ATfsCharacter::GetAbilitySystemComponent() const
{
	return FtsAbilitySystemComponent;
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

