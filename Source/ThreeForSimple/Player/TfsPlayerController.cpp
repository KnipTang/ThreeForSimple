// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsPlayerController.h"

#include "TfsPlayerCharacter.h"

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
	}
}
