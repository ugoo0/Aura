// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
public:
	FDamageEffectParams(){}
		
	UPROPERTY()
	TObjectPtr<UObject> WorldContext = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	UPROPERTY()
	float BaseDamage = 0.f;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackChance = 0.f;
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext :  public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	bool GetIsCriticalHit() const { return bIsCriticalHit;}
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit;}
	
	bool GetIsBlockHit() const { return bIsBlockHit;}
	void SetIsBlockHit(bool bInIsBlockHit) { bIsBlockHit = bInIsBlockHit;}

	bool GetIsSuccessfulDebuff() const { return bIsSuccessfulDebuff;}
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff;}
	
	float GetDebuffDamage() const { return DebuffDamage;}
	void SetDebuffDamage(float bInDebuffDamage) { DebuffDamage = bInDebuffDamage;}

	float GetDebuffDuration() const { return DebuffDuration;}
	void SetDebuffDuration(float bInDebuffDuration) { DebuffDuration = bInDebuffDuration;}
	
	float GetDebuffFrequency() const { return DebuffFrequency;}
	void SetDebuffFrequency(float bInDebuffFrequency) { DebuffFrequency = bInDebuffFrequency;}

	FVector GetDeathImpulse() const { return DeathImpulse;}
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse;}

	FVector GetKnockbackForce() const { return KnockbackForce;}
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce;}
	
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType;}
	void SetDamageType(const TSharedPtr<FGameplayTag>& bInDamageType) { DamageType =  bInDamageType;}
	
	virtual FAuraGameplayEffectContext* Duplicate() const override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
protected:

private:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};