// Fill out your copyright notice in the Description page of Project Settings.


#include "TfsGameInstance.h"

void UTfsGameInstance::StartGame() const
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer)
		LoadLevel(GameLevel);
}

void UTfsGameInstance::LoadLevel(const TSoftObjectPtr<UWorld>& LevelToLoad) const
{
	const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(LevelToLoad.ToString()));
	if (LevelURL != "")
	{
		GetWorld()->ServerTravel(LevelURL.ToString() + "?listen");
	}
}
