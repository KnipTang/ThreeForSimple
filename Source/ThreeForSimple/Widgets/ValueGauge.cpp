// Fill out your copyright notice in the Description page of Project Settings.


#include "ValueGauge.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!ProgressBar)
		return;
    		
	ProgressBar->SetFillColorAndOpacity(BarColor);
}

void UValueGauge::SetAndBoundToGameplayAttribute(class UAbilitySystemComponent* ASC,
	const struct FGameplayAttribute& Attribute, const struct FGameplayAttribute& MaxAttribute)
{
	if (ASC)
	{
		bool bFoundValue;
		bool bFoundMaxValue;
		float Value = ASC->GetGameplayAttributeValue(Attribute, bFoundValue);
		float MaxValue = ASC->GetGameplayAttributeValue(MaxAttribute, bFoundMaxValue);
		if (bFoundValue && bFoundMaxValue)
		{
			SetValue(Value, MaxValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ValueNotFound"));
		}
		
		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UValueGauge::ValueChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UValueGauge::MaxValueChanged);
	}
}

void UValueGauge::SetValue(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;
	
	if (NewMaxValue == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Value is zero"));
		return;
	}
	
	float NewPercent = NewValue / NewMaxValue;
	ProgressBar->SetPercent(NewPercent);

	FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);

	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormattingOptions),
			FText::AsNumber(NewMaxValue, &FormattingOptions)
		)
	);
}

void UValueGauge::ValueChanged(const struct FOnAttributeChangeData& ChangedData)
{
	SetValue(ChangedData.NewValue, CachedMaxValue);
}

void UValueGauge::MaxValueChanged(const struct FOnAttributeChangeData& ChangedData)
{
	SetValue(CachedValue, ChangedData.NewValue);
}
