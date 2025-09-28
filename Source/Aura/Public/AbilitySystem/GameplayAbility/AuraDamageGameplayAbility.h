// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FDamageEffectParams GetDamageEffectParamsForTarget(AActor* Target = nullptr) const;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	FScalableFloat DamageScalableFloat;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float DebuffDamage = 2.0f;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float DebuffDuration = 5.0f;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float DebuffFrequency = 1.0f;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float DeathImpulseMagnitude = 1000.0f;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float KnockbackForceMagnitude = 1000.0f;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float KnockbackChance = 20.f;
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void CauseDamageToTarget(AActor* Target, UPARAM(ref) const TArray<FName>& TagsGetHurt);

	UFUNCTION(BlueprintCallable, Category="Combat")
	float GetDamageByLevel(int32 Level) const;
	
};
