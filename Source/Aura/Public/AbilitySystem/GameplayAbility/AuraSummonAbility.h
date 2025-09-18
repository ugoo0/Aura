// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClass;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<APawn> GetSpawnClassRandom();
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MinSpawnDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MaxSpawnDistance = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float SpawnSpread = 90.f;
};
