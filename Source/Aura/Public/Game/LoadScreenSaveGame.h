// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT()
struct FSaveActorInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSaveActorInfo& Left, const FSaveActorInfo& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	UPROPERTY()
	FString MapName = FString();

	UPROPERTY()
	TArray<FSaveActorInfo> Actors;
};

USTRUCT(BlueprintType)
struct FSaveAbilityInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	int32 AbilityLevel = 1;
};

inline bool operator==(const FSaveAbilityInfo& Left, const FSaveAbilityInfo& Right)
{
	return Left.AbilityType.MatchesTagExact(Right.AbilityTag);
}

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> LoadSlotStatus;
	
	UPROPERTY()
	FString CurMapName;

	UPROPERTY()
	FName PlayerStartTag;


	UPROPERTY()
	bool IsFirstSaveData = true;
	/* Player */

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;

	UPROPERTY()
	float Resilience = 0;

	UPROPERTY()
	float Vigor = 0;

	UPROPERTY()
	TArray<FSaveAbilityInfo> SaveAbilities;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	bool HasMap(const FString& InMapName);
	FSavedMap GetSavedMap(const FString& InMapName);
};
