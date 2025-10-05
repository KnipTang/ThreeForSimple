// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestInteractable.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"
#include "ThreeForSimple/Player/TfsPlayerController.h"
#include "ThreeForSimple/Widgets/Items/LootChest/LootChestWidget.h"

void ALootChestInteractable::BeginPlay()
{
	Super::BeginPlay();

	LootChestWidget = CreateWidget<ULootChestWidget>(GetWorld());
}

void ALootChestInteractable::OnInteract(AActor* InteractActor)
{
	Super::OnInteract(InteractActor);

	//if (ACharacter* Character = Cast<ACharacter>(InteractActor))
	//	if (APlayerController* PlayerController = Character->GetController<APlayerController>())
	//		if (ATfsPlayerController* TfsPlayerController = Cast<ATfsPlayerController>(PlayerController))
	//			TfsPlayerController->SetLootChestWidget(LootChestWidget);
}
