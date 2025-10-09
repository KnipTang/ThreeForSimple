// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TfsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THREEFORSIMPLE_API UTfsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void StartGame() const;
private:
	void LoadLevel(const TSoftObjectPtr<UWorld>& LevelToLoad) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> LobbyLevel;
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> GameLevel;
};
