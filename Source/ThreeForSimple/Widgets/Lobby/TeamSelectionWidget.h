// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamSelectionWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, uint8 /*SlotID*/)
/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTeamSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	void SetSlotID(uint8 NewSlotID);
	void UpdateNickName(const FText& PlayerNickName);
	
	FOnSlotClicked OnSlotClicked;
private:
	UPROPERTY(meta=(BindWidget))
	class UButton* SelectButton;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PlayerNickNameText;

	UFUNCTION()
	void SelectButtonClicked();

	uint8 SlotID;
};
