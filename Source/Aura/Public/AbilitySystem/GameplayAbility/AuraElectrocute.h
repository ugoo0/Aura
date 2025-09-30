// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
