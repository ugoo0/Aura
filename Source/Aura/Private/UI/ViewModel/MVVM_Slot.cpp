// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_Slot::InitializeSlot()
{
	SetWidgetSwitcherIndex.Broadcast(LoadSlotStatus.GetIntValue());
}

void UMVVM_Slot::SetLoadSlotName(const FString& SlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, SlotName);
}

void UMVVM_Slot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_Slot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

