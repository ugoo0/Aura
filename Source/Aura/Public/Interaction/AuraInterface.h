// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAuraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAuraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForExperience(int32 Experience) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 CurLevel, int32 NewLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 CurLevel, int32 NewLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp(int32 CurLevel, int32 NewLevel);
	
};
