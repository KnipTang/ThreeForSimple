// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestInteractable.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "ThreeForSimple/Inventory/InventoryComponent.h"
#include "ThreeForSimple/Player/TfsPlayerController.h"
#include "ThreeForSimple/Widgets/Items/Inventory/InventoryWidget.h"

ALootChestInteractable::ALootChestInteractable()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
}

void ALootChestInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void ALootChestInteractable::OnInteract(AActor* InteractActor)
{
	Super::OnInteract(InteractActor);

	if (ACharacter* Character = Cast<ACharacter>(InteractActor))
		if (APlayerController* PlayerController = Character->GetController<APlayerController>())
			if (ATfsPlayerController* TfsPlayerController = Cast<ATfsPlayerController>(PlayerController))
			{
				if (LootChestWidgetClass)
				{
					LootChestWidget = CreateWidget<UInventoryWidget>(TfsPlayerController, LootChestWidgetClass);
				}
	
				if (InventoryComponent && LootChestWidget)
				{
					LootChestWidget->InventoryComponent = InventoryComponent;
					for (const UPA_LootChestItem* LootChestItem : LootChestItems)
						InventoryComponent->TryAddToInventory(LootChestItem);
				}
				
				TfsPlayerController->SetLootChestWidget(LootChestWidget);
			}
}
