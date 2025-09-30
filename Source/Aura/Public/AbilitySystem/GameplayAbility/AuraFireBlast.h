// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"


class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumOfFirebolt = 12;


	UFUNCTION(BlueprintImplementableEvent)
	void OnFinalExplode(const AActor* Target, const FDamageEffectParams& Params);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
