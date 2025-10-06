// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Class")
	ECharacterClassType CharacterClass = ECharacterClassType::Warrior;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
};
