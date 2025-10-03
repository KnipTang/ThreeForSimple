// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ThreeForSimple/GAS/TfsAbilitySystemStatics.h"

ATfsPlayerCharacter::ATfsPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	
	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.0f,720.0f,0.0f );
}

void ATfsPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	
	if (APlayerController* OwningPlayerController = GetController<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->RemoveMappingContext(GameplayInputMappingContext);
			InputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
		}
	}
}

void ATfsPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATfsPlayerCharacter::HandleMoveInput);
		EnhancedInputComp->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ATfsPlayerCharacter::HandleLookInput);
		EnhancedInputComp->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ATfsPlayerCharacter::Jump);

		for (const TPair<ECAbilityInputID, class UInputAction*>& GameplayAbilityInputAction : GameplayAbilitiesInputAction)
			EnhancedInputComp->BindAction(GameplayAbilityInputAction.Value, ETriggerEvent::Triggered, this, &ATfsPlayerCharacter::HandleAbilityInput, GameplayAbilityInputAction.Key);
	}
}

void ATfsPlayerCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = ViewCam->GetComponentLocation();
	OutRotation = ViewCam->GetComponentRotation();
}

void ATfsPlayerCharacter::HandleMoveInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputVal = InputActionValue.Get<FVector2D>();
	InputVal.Normalize();

	FVector LookRightDir = ViewCam->GetRightVector();
	FVector MoveForwardDir = FVector::CrossProduct(LookRightDir, FVector::UpVector);
	AddMovementInput(MoveForwardDir * InputVal.Y + LookRightDir * InputVal.X);
}

void ATfsPlayerCharacter::HandleLookInput(const struct FInputActionValue& InputActionValue)
{
	FVector2D InputVal = InputActionValue.Get<FVector2D>();

	AddControllerPitchInput(-InputVal.Y);
	AddControllerYawInput(InputVal.X);
}

void ATfsPlayerCharacter::HandleAbilityInput(const struct FInputActionValue& InputActionValue, const ECAbilityInputID AbilityInputID)
{
	bool bPressed = InputActionValue.Get<bool>();
	
	if (bPressed)
		GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(AbilityInputID));
	else
		GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(AbilityInputID));

	if (AbilityInputID == ECAbilityInputID::BasicAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("BasicPressed"));

		FGameplayTag BasicAttackTag = bPressed ? UTfsAbilitySystemStatics::GetBasicAttackInputPressedTag() : UTfsAbilitySystemStatics::GetBasicAttackInputReleasedTag();
		//This only gets called on the client. Therefor another function (Server_SendGameplayEventToSelf in CCharacter) Is needed to communicate this to the server
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, BasicAttackTag, FGameplayEventData());
		Server_SendGameplayEventToSelf(BasicAttackTag, FGameplayEventData());
	}
}

void ATfsPlayerCharacter::OnDead()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		DisableInput(PlayerController);
	}
}

void ATfsPlayerCharacter::OnRespawn()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		EnableInput(PlayerController);
	}
}

void ATfsPlayerCharacter::LerpCameraToLocalOffsetLocation(const FVector& LerpedCameraLocGoal)
{
	GetWorldTimerManager().ClearTimer(CameraLerpTimerHandle);
	CameraLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ATfsPlayerCharacter::TickCameraLocalOffsetLerp, LerpedCameraLocGoal));
}

void ATfsPlayerCharacter::TickCameraLocalOffsetLerp(const FVector LerpedCameraLocGoal)
{
	FVector CurrentLocalOffset = ViewCam->GetRelativeLocation();
	//If the camera is pretty close to the goal location just set it.
	if(FVector::Dist(CurrentLocalOffset, LerpedCameraLocGoal) < 1.f)
	{
		ViewCam->SetRelativeLocation(LerpedCameraLocGoal);
		return;
	}

	float LerpAlpha = FMath::Clamp(GetWorld()->GetDeltaSeconds() * CameraLerpSpeed, 0.f, 1.f);
	FVector NewLocalOffset = FMath::Lerp(CurrentLocalOffset, LerpedCameraLocGoal, LerpAlpha);
	ViewCam->SetRelativeLocation(NewLocalOffset);

	CameraLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ATfsPlayerCharacter::TickCameraLocalOffsetLerp, LerpedCameraLocGoal));
}

void ATfsPlayerCharacter::OnAimStateChanged(const bool bIsAiming)
{
	LerpCameraToLocalOffsetLocation(bIsAiming ? CameraAimLocalOffset : FVector{0.f});
}
