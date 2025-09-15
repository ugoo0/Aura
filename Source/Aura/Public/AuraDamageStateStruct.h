// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageStateStruct.generated.h" 


/**
 * 
 */
USTRUCT(BlueprintType)
struct FDamageState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	bool IsCriticalHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool IsBlockHit = false;
};
