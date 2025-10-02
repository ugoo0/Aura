// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_LoadScreen::InitialLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlots.Add(2, LoadSlot_2);
}

void UMVVM_LoadScreen::NewSlotButtonClicked(int32 Slot, const FString& EnterName)
{
}

void UMVVM_LoadScreen::NewGameButtonClicked(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectButtonClicked(int32 Slot)
{
}

void UMVVM_LoadScreen::SetLoadSlotName(const FString& SlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, SlotName);
}
