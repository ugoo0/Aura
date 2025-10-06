// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AuraSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_Slot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->LoadSlotStatus = ESaveSlotStatus::Taken;
	LoadScreenSaveGame->CurMapName =  LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag =  LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->CurMapAssetName = LoadSlot->MapAssetName;
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	ULoadScreenSaveGame* LoadScreenSaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,SlotIndex));
	}
	else
	{
		LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass));
		LoadScreenSaveGame->CurMapName = DefaultMapName;
	}
	return  LoadScreenSaveGame;
}

ULoadScreenSaveGame* AAuraGameModeBase::GetCurGameInstanceSaveData() const
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	return GetSaveSlotData(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
}

void AAuraGameModeBase::SavingCurGameInstanceProgress(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
	UGameplayStatics::SaveGameToSlot(SaveObject, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMapBySlot(UMVVM_Slot* Slot) const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	if (PlayerStarts.Num() > 0)
	{
		AActor* SelectedPlayerStart = PlayerStarts[0];
		for (AActor* Actor : PlayerStarts)
		{
			if (APlayerStart*  PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedPlayerStart = Actor;
					return SelectedPlayerStart;
				}
			}
		}
		return SelectedPlayerStart;
	}
	
	return nullptr;
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	FString MapName = UGameplayStatics::GetCurrentLevelName(World);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance)

	if (ULoadScreenSaveGame* LoadScreenSaveGame = GetSaveSlotData(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			LoadScreenSaveGame->CurMapAssetName = DestinationMapAssetName;
			LoadScreenSaveGame->CurMapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		if (!LoadScreenSaveGame->HasMap(MapName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapName = MapName;
			LoadScreenSaveGame->SavedMaps.Add(NewSavedMap);
		}
		FSavedMap SavedMap = LoadScreenSaveGame->GetSavedMap(MapName);
		SavedMap.Actors.Empty();

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<UAuraSaveInterface>()) continue;

			FSaveActorInfo SaveActor;
			SaveActor.ActorName = Actor->GetFName();
			SaveActor.Transform = Actor->GetActorTransform();

			FMemoryWriter MemoryWriter(SaveActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;

			Actor->Serialize(Archive);
			SavedMap.Actors.AddUnique(SaveActor);
		}

		for (FSavedMap& MapToReplace : LoadScreenSaveGame->SavedMaps)
		{
			if (MapToReplace.MapName == MapName)
			{
				MapToReplace = SavedMap;
			}
		}

		UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
	}

	
}

void AAuraGameModeBase::LoadWorldState(UWorld* World)
{
	FString MapName = UGameplayStatics::GetCurrentLevelName(World);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance)

	if (UGameplayStatics::DoesSaveGameExist(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
	{

		ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex));
		if (!LoadScreenSaveGame)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UAuraSaveInterface>()) continue;

			for (FSaveActorInfo& SaveActor : LoadScreenSaveGame->GetSavedMap(MapName).Actors)
			{
				if (Actor->GetFName() == SaveActor.ActorName)
				{
					if (IAuraSaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SaveActor.Transform);
					}
					FMemoryReader MemoryReader(SaveActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);
					IAuraSaveInterface::Execute_LoadData(Actor);
				}
			}
		}
		
		
	}

}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& InMapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == InMapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AAuraGameModeBase::PlayerDie(ACharacter* Player)
{
	if (!IsValid(Player)) return;
	UGameplayStatics::OpenLevel(Player,FName(GetCurGameInstanceSaveData()->CurMapAssetName));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
