// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="LevelInfo")
	TArray<FAuraLevelInfo> Levels;

	UFUNCTION(BlueprintCallable)
	int32 FindLevelForExperience(int32 Experience);

	float GetShowPercentForExperience(int32 Experience);

	FAuraLevelInfo GetLevelInfoForLevel(int32 InLevel);
};
