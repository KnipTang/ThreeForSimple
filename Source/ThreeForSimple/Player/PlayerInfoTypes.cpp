// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoTypes.h"

#include "GameFramework/PlayerState.h"

FPlayerSelection::FPlayerSelection() :
	SlotID { GetInvalidSlot() },
	PlayerUniqueId{ FUniqueNetIdRepl::Invalid() },
	PlayerNickName {}
{
}

FPlayerSelection::FPlayerSelection(uint8 InSlot, const APlayerState* InPlayerState) :
	SlotID { InSlot }
{
	if (InPlayerState)
	{
		PlayerUniqueId = InPlayerState->GetUniqueId();
		PlayerNickName = FText::FromString(InPlayerState->GetPlayerName());
	}
}

bool FPlayerSelection::IsPlayerLinkedToSelection(const APlayerState* InPlayerState) const
{
	if (!InPlayerState)
		return false;

	#if WITH_EDITOR
		return InPlayerState->GetPlayerName() == PlayerNickName.ToString();
	#else
		return InPlayerState->GetUniqueId() == GetPLayerUniqueId();
	#endif
}

uint8 FPlayerSelection::GetInvalidSlot()
{
	return 255;
}
