// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets);
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Beam Spell")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Beam Spell")
	TObjectPtr<AActor> MouseHitActor;

	
	UPROPERTY(BlueprintReadWrite, Category = "Beam Spell")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category = "Beam Spell")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Beam Spell")
	float BeamRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Beam Spell")
	int32 MaxBeamSpreadNum = 5;
};
