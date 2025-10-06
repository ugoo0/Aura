// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ViewModel/MVVM_Slot.h"
#include "AuraGameModeBase.generated.h"

class ULootTiers;
class USaveGame;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/* Data Asset */
	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category="LootTiers")
	TObjectPtr<ULootTiers> LootTiers;
	/* end Data Asset */
	
	void SaveSlotData(UMVVM_Slot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	UFUNCTION()
	ULoadScreenSaveGame* GetCurGameInstanceSaveData() const;

	UFUNCTION()
	void SavingCurGameInstanceProgress(ULoadScreenSaveGame* SaveObject);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UFUNCTION()
	static  void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	FString MapAssetName;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UFUNCTION()
	void TravelToMapBySlot(UMVVM_Slot* Slot) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void SaveWorldState(UWorld* World,const FString& DestinationMapAssetName = FString(""));
	void LoadWorldState(UWorld* World);
	
	FString GetMapNameFromMapAssetName(const FString& InMapAssetName) const;

	UFUNCTION()
	void PlayerDie(ACharacter* Player);
protected:
	virtual void BeginPlay() override;
};
