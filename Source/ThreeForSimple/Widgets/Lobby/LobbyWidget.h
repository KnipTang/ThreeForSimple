// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta=(BindWidget))
	class UButton* StartGameButton;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* StartGameButtonText;

	UPROPERTY(meta=(BindWidget))
	class UUniformGridPanel* TeamSelectionSlotGridPanel;

private:
	void PopulateTeamSelectionSlots();
	void SlotSelected(const uint8 NewSlotID);

	//Recursively calls this function until TFS game state has been found
	void ConfigureGameState();
	FTimerHandle ConfigGameStateTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "GameState")
	float RetryToFindGameStateRate = 1.f;
	class ATfsGameState* TfsGameState;

	void UpdatePlayerSelectionDisplay(const TArray<struct FPlayerSelection>& PlayerSelections);
	
	UPROPERTY(EditDefaultsOnly, Category = "TeamSelection")
	TSubclassOf<class UTeamSelectionWidget> TeamSelectionWidgetClass;
	UPROPERTY()
	TArray<class UTeamSelectionWidget*> TeamSelectionSlots;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FText StartButtonTextOnDisabled = FText::FromString("Every player should pick a team");
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FText StartButtonTextOnEnabled = FText::FromString("Start Game");
	
	class ALobbyPlayerController* LobbyPlayerController;

	UFUNCTION()
	void StartGameButtonClicked();
};
