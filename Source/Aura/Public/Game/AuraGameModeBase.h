// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ViewModel/MVVM_Slot.h"
#include "AuraGameModeBase.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Character Class Default")
	TObjectPtr<UAbilityInfo> AbilityInfo;

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
	FName DefaultPlayerStartTag;
	
	UFUNCTION()
	void TravelToMapBySlot(UMVVM_Slot* Slot) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void SaveWorldState(UWorld* World);
	void LoadWorldState(UWorld* World);
	
protected:
	virtual void BeginPlay() override;
};
