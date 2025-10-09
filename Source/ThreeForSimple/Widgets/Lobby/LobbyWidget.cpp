// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "TeamSelectionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "ThreeForSimple/Framework/TfsGameState.h"
#include "ThreeForSimple/Network/TfsNetworkStatics.h"
#include "ThreeForSimple/Player/PlayerInfoTypes.h"
#include "ThreeForSimple/Player/Menu/LobbyPlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameButton->AddChild(StartGameButtonText);
	StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidget::StartGameButtonClicked);
	
	PopulateTeamSelectionSlots();
	LobbyPlayerController = GetOwningPlayer<ALobbyPlayerController>();
	ConfigureGameState();
}

void ULobbyWidget::PopulateTeamSelectionSlots()
{
	TeamSelectionSlotGridPanel->ClearChildren();

	for (int i = 0; i < UTfsNetworkStatics::GetMaxPlayerCountPerTeam() * 2; i++)
	{
		if (UTeamSelectionWidget* NewTeamSelectionSlot = CreateWidget<UTeamSelectionWidget>(this, TeamSelectionWidgetClass))
		{
			NewTeamSelectionSlot->SetSlotID(i);
			
			if (UUniformGridSlot* NewUniformGridSlot = TeamSelectionSlotGridPanel->AddChildToUniformGrid(NewTeamSelectionSlot))
			{
				const int Row = i % UTfsNetworkStatics::GetMaxPlayerCountPerTeam();
				const int Column = i < UTfsNetworkStatics::GetMaxPlayerCountPerTeam() ? 0 : 1;
				NewUniformGridSlot->SetRow(Row);
				NewUniformGridSlot->SetColumn(Column);
			}

			NewTeamSelectionSlot->OnSlotClicked.AddUObject(this, &ULobbyWidget::SlotSelected);
			TeamSelectionSlots.Add(NewTeamSelectionSlot);
		}
	}
}

void ULobbyWidget::SlotSelected(const uint8 NewSlotID)
{
	UE_LOG(LogTemp, Display, TEXT("SlotSelected %d"), NewSlotID);
	if (LobbyPlayerController)
		LobbyPlayerController->Server_RequestSlotSelectionChange(NewSlotID);
}

void ULobbyWidget::ConfigureGameState()
{
	const UWorld* World = GetWorld();
	if (!World)
		return;

	TfsGameState = World->GetGameState<ATfsGameState>();
	if (!TfsGameState)
		World->GetTimerManager().SetTimer(ConfigGameStateTimerHandle, this, &ULobbyWidget::ConfigureGameState, RetryToFindGameStateRate);
	else
	{
		TfsGameState->OnPlayerSelectionUpdated.AddUObject(this, &ULobbyWidget::UpdatePlayerSelectionDisplay);
		//If player joins later
		UpdatePlayerSelectionDisplay(TfsGameState->GetPlayerSelection());
	}
}

void ULobbyWidget::UpdatePlayerSelectionDisplay(const TArray<struct FPlayerSelection>& PlayerSelections)
{
	for (UTeamSelectionWidget* SelectionSlot : TeamSelectionSlots)
	{
		SelectionSlot->UpdateNickName(FText::FromString("None"));
	}

	for (const FPlayerSelection& PlayerSelection : PlayerSelections)
	{
		TeamSelectionSlots[PlayerSelection.GetSlotID()]->UpdateNickName(PlayerSelection.GetPlayerNickName());
	}

	if (TfsGameState)
	{
		bool bIsGameReadyToStart = TfsGameState->CanStartGame();
		StartGameButton->SetIsEnabled(bIsGameReadyToStart);
		bIsGameReadyToStart ? StartGameButtonText->SetText(StartButtonTextOnEnabled) : StartGameButtonText->SetText(StartButtonTextOnDisabled);
	}
}

void ULobbyWidget::StartGameButtonClicked()
{
	if (LobbyPlayerController)
		LobbyPlayerController->Server_RequestStartGame();
}
