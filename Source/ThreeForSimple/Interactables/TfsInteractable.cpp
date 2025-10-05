// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsInteractable.h"

// Sets default values
ATfsInteractable::ATfsInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneRoot;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionObjectType(ECC_WorldStatic);
	StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATfsInteractable::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATfsInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATfsInteractable::OnInteract(AActor* InteractActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting %s"), *this->GetName());
}

