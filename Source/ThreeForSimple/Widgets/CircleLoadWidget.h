// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CircleLoadWidget.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActivation, const bool /*bIsActivate*/, const float /*TotalCircleLoadTime*/);

UCLASS()
class THREEFORSIMPLE_API UCircleLoadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FOnActivation OnActivation;
private:
	UPROPERTY(meta=(BindWidget))
	class UImage* CircleLoadImage;

	void UpdateCircle(float DeltaTime);
	
	void CircleLoadTagUpdated(const bool bIsVisible) const;
	void UpdateCircleLoadPosition();
	void CircleLoadUpdateFill(const float Value = 1.f);

	UFUNCTION()
	void SetDecreaseFillRate(const bool bIsActivate, const float TotalCircleLoadTimeSeconds);

	UPROPERTY(EditDefaultsOnly, Category="Config")
	float StartFill = 1.f;
	float CurrentFill = 0.f;
	float DecreaseFillRate = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category="Config")
	UMaterialInterface* Material;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	
	UPROPERTY()
	class UCanvasPanelSlot* CircleLoadCanvasPanelSlot;

	UPROPERTY()
	class APlayerController* CachedPlayerController;
};
