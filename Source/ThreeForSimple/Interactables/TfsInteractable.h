// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "TfsInteractable.generated.h"

UCLASS()
class THREEFORSIMPLE_API ATfsInteractable : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATfsInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract(AActor* InteractActor) override;
private:
	UPROPERTY()
	class USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;
};
