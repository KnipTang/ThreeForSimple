// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsCharacter.h"

#include "ThreeForSimple/GAS/TfsAbilitySystemComponent.h"
#include "ThreeForSimple/GAS/TfsAttributeSet.h"

// Sets default values
ATfsCharacter::ATfsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FtsAbilitySystemComponent = CreateDefaultSubobject<UTfsAbilitySystemComponent>("Ability System Component");
	FtsAttributeSet = CreateDefaultSubobject<UTfsAttributeSet>("Attribute Set");
}

void ATfsCharacter::ServerSideInit()
{
	FtsAbilitySystemComponent->InitAbilityActorInfo(this, this);
	FtsAbilitySystemComponent->ApplyInitialEffects();
}

void ATfsCharacter::ClientSideInit()
{
	FtsAbilitySystemComponent->InitAbilityActorInfo(this, this);
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

