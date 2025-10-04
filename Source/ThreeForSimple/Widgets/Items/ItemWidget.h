// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void SetIcon(UTexture2D* IconTexture);
private:
	UPROPERTY(meta=(bindWidget))
	class UImage* ItemIcon;

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

	virtual void RightMouseButtonClicked() {UE_LOG(LogTemp, Warning, TEXT("RightMouseButtonClicked"));};
	virtual void LeftMouseButtonClicked() {UE_LOG(LogTemp, Warning, TEXT("LeftMouseButtonClicked"));};;
};
