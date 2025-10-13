// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleLoadWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

void UCircleLoadWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	CircleLoadUpdateFill(StartFill);
}

void UCircleLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleLoadImage->SetVisibility(ESlateVisibility::Hidden);
	
	CachedPlayerController = GetOwningPlayer();
	
	CircleLoadCanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (!CircleLoadCanvasPanelSlot)
		UE_LOG(LogTemp, Error, TEXT("Crosshair widget need to be parented under a canvas panel to properly place itself"));

	OnActivation.AddUObject(this, &UCircleLoadWidget::SetDecreaseFillRate);
}

void UCircleLoadWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentFill > 0)
		UpdateCircle(InDeltaTime);
}

void UCircleLoadWidget::CircleLoadUpdateFill(const float Value)
{
	if (!DynamicMaterial)
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

	CircleLoadImage->SetBrushFromMaterial(DynamicMaterial);

	DynamicMaterial->SetScalarParameterValue(FName("Percentage"), Value);
}

void UCircleLoadWidget::SetDecreaseFillRate(const bool bIsActivate, const float TotalCircleLoadTimeSeconds)
{
	DecreaseFillRate = TotalCircleLoadTimeSeconds;

	bIsActivate ? CurrentFill = StartFill : CurrentFill = 0;

	CircleLoadTagUpdated(bIsActivate);
}

void UCircleLoadWidget::UpdateCircle(float DeltaTime)
{
	UpdateCircleLoadPosition();
	
	CurrentFill -= DeltaTime / DecreaseFillRate;
	CircleLoadUpdateFill(CurrentFill);
}

void UCircleLoadWidget::CircleLoadTagUpdated(const bool bIsVisible) const
{
	CircleLoadImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCircleLoadWidget::UpdateCircleLoadPosition()
{
	if (!CachedPlayerController || !CircleLoadCanvasPanelSlot)
		return;
	
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	int32 SizeX, SizeY;
	CachedPlayerController->GetViewportSize(SizeX, SizeY);
	FVector2D ViewportSize = FVector2D{static_cast<float>(SizeX), static_cast<float>(SizeY)};
	CircleLoadCanvasPanelSlot->SetPosition(ViewportSize / 2.f / ViewportScale);
}
