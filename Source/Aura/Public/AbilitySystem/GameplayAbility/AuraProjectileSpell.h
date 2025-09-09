// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& TargetLocation);
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
private:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;


};
