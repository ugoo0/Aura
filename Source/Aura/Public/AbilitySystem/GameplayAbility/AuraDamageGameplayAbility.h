// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TMap<FGameplayTag,FScalableFloat>  DamageTypes;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void CauseDamageToTarget(AActor* Target, UPARAM(ref) const TArray<FName>& TagsGetHurt);

	int32 GetDamageByLevelAndDamageType(int32 Level, const FGameplayTag& DamageType) const;
	
};
