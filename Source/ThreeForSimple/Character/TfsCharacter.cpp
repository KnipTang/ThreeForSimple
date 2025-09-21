// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsCharacter.h"

// Sets default values
ATfsCharacter::ATfsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATfsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATfsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATfsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

