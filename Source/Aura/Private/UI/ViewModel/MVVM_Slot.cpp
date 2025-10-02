// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_Slot::InitializeSwitcherSlot()
{
	SetWidgetSwitcherIndex.Broadcast(0);
}

void UMVVM_Slot::SetLoadSlotName(const FString& SlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, SlotName);
}

