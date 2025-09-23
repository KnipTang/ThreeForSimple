// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsPlayerController.h"

#include "TfsPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "ThreeForSimple/Widgets/GameplayWidget.h"

void ATfsPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	TfsPlayerCharacter = Cast<ATfsPlayerCharacter>(NewPawn);
	if (TfsPlayerCharacter)
	{
		TfsPlayerCharacter->ServerSideInit();
	}
}

void ATfsPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);

	TfsPlayerCharacter = Cast<ATfsPlayerCharacter>(NewPawn);
	if (TfsPlayerCharacter)
	{
		TfsPlayerCharacter->ClientSideInit();
		SpawnGameplayWidget();
	}
}

void ATfsPlayerController::SpawnGameplayWidget()
{
	//Check if it's not a dedicated server or listening server instance
	if (!IsLocalPlayerController())
		return;

	if (!GameplayWidgetClass)
		return;
	
	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}
