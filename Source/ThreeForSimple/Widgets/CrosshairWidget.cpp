// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairImage->SetVisibility(ESlateVisibility::Hidden);

	if (UAbilitySystemComponent* OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn()))
	{
		OwnerAbilitySystemComponent->RegisterGameplayTagEvent(UTfsAbilitySystemStatics::GetCrosshairStatTag()).AddUObject(this, &UCrosshairWidget::CrosshairTagUpdated);
		OwnerAbilitySystemComponent->GenericGameplayEventCallbacks.Add(UTfsAbilitySystemStatics::GetTargetUpdatedTag()).AddUObject(this, &UCrosshairWidget::TargetUpdated);
	}

	CachedPlayerController = GetOwningPlayer();
	
	CrosshairCanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (!CrosshairCanvasPanelSlot)
		UE_LOG(LogTemp, Error, TEXT("Crosshair widget need to be parented under a canvas panel to properly place itself"));
}

void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CrosshairImage->GetVisibility() == ESlateVisibility::Visible)
		UpdateCrosshairPosition();
}

void UCrosshairWidget::CrosshairTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	CrosshairImage->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCrosshairWidget::UpdateCrosshairPosition()
{
	if (!CachedPlayerController || !CrosshairCanvasPanelSlot)
		return;
	
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	int32 SizeX, SizeY;
	CachedPlayerController->GetViewportSize(SizeX, SizeY);
	FVector2D ViewportSize = FVector2D{static_cast<float>(SizeX), static_cast<float>(SizeY)};
	CrosshairCanvasPanelSlot->SetPosition(ViewportSize / 2.f / ViewportScale);
}

void UCrosshairWidget::TargetUpdated(const struct FGameplayEventData* EventData)
{
	AimTarget = EventData->Target;
	CrosshairImage->SetColorAndOpacity(AimTarget ? HasTargetColor : NoTargetColor);
}
