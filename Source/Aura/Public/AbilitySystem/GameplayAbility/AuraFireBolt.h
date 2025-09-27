// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	UFUNCTION(BlueprintCallable, Category="Projectiles")
	void SpawnProjectiles(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float OverridePitch = 0.f, AActor* HomingTarget = nullptr);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles")
	float Spread = 90.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Projectiles")
	int32 MaxSpreadNums = 5;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Projectiles")
	int32 SpreadNums = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles")
	bool bIsHoming = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectiles")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingSceneComponent;
};
