// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueGauge.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UValueGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void SetAndBoundToGameplayAttribute(class UAbilitySystemComponent* ASC, const struct FGameplayAttribute& Attribute, const struct FGameplayAttribute& MaxAttribute);
	void SetValue(float NewValue, float NewMaxValue);

private:
	void ValueChanged(const struct FOnAttributeChangeData& ChangedData);
	void MaxValueChanged(const struct FOnAttributeChangeData& ChangedData);
	
private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor BarColor;
	UPROPERTY(VisibleAnywhere, meta=(bindWidget))
	class UProgressBar* ProgressBar;
	UPROPERTY(VisibleAnywhere, meta=(bindWidget))
	class UTextBlock* ValueText;

	float CachedValue;
	float CachedMaxValue;
};
