// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSelectionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTeamSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectButton->OnClicked.AddDynamic(this, &UTeamSelectionWidget::SelectButtonClicked);
}

void UTeamSelectionWidget::SetSlotID(uint8 NewSlotID)
{
	SlotID = NewSlotID;
}

void UTeamSelectionWidget::UpdateNickName(const FText& PlayerNickName)
{
	PlayerNickNameText->SetText(PlayerNickName);
}

void UTeamSelectionWidget::SelectButtonClicked()
{
	OnSlotClicked.Broadcast(SlotID);
}
