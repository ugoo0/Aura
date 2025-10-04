// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_LoadScreen::InitialLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_Slot>(this, LoadSlotClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

void UMVVM_LoadScreen::NewSlotButtonClicked(int32 Slot, const FString& EnterName)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(EnterName);
	LoadSlots[Slot]->SetMapName(AuraGameModeBase->DefaultMapName);
	LoadSlots[Slot]->LoadSlotStatus = ESaveSlotStatus::Taken;
	LoadSlots[Slot]->PlayerStartTag = AuraGameModeBase->DefaultPlayerStartTag;
	LoadSlots[Slot]->SetPlayerLevel(1);
	AuraGameModeBase->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
	

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	AuraGameInstance->PlayerStartTag = AuraGameModeBase->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::NewGameButtonClicked(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
	EnablePlayAndDelete.Broadcast(false);
	
}

void UMVVM_LoadScreen::SelectButtonClicked(int32 Slot)
{
	SelectedSlot = LoadSlots[Slot];
	EnablePlayAndDelete.Broadcast(true);
	for (const TTuple<int32, UMVVM_Slot*> LoadSlot : LoadSlots)
	{
		if (Slot == LoadSlot.Key)
		{
			LoadSlot.Value->EnableButtonStatus.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableButtonStatus.Broadcast(true);
		}
	}
}

void UMVVM_LoadScreen::DeleteSlot()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->LoadSlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableButtonStatus.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonClick()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		AuraGameModeBase->TravelToMapBySlot(SelectedSlot);
	}
}

void UMVVM_LoadScreen::SetLoadSlotName(const FString& SlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, SlotName);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_Slot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* LoadScreenSaveGame =  AuraGameModeBase->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		LoadSlot.Value->LoadSlotStatus = LoadScreenSaveGame->LoadSlotStatus;
		LoadSlot.Value->SetPlayerName(LoadScreenSaveGame->PlayerName);
		LoadSlot.Value->SetMapName(LoadScreenSaveGame->CurMapName);
		LoadSlot.Value->PlayerStartTag = LoadScreenSaveGame->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(LoadScreenSaveGame->Level);
		LoadSlot.Value->InitializeSlot();
	}
}
