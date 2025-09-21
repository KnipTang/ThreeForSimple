// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreeForSimple/Character/TfsCharacter.h"
#include "TfsPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API ATfsPlayerCharacter : public ATfsCharacter
{
	GENERATED_BODY()

public:
	ATfsPlayerCharacter();

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class UCameraComponent* ViewCam;

	/***************************************************/
	/*						INPUT						/
	/***************************************************/
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleMoveInput(const struct FInputActionValue& InputActionValue);
	void HandleLookInput(const struct FInputActionValue& InputActionValue);
};
