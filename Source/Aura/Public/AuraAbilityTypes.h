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
		
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContext = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext :  public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	/*Damage Show States*/
	bool GetIsCriticalHit() const { return bIsCriticalHit;}
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit;}
	
	bool GetIsBlockHit() const { return bIsBlockHit;}
	void SetIsBlockHit(bool bInIsBlockHit) { bIsBlockHit = bInIsBlockHit;}

	/*Debuff*/
	bool GetIsSuccessfulDebuff() const { return bIsSuccessfulDebuff;}
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff;}
	
	float GetDebuffDamage() const { return DebuffDamage;}
	void SetDebuffDamage(float bInDebuffDamage) { DebuffDamage = bInDebuffDamage;}

	float GetDebuffDuration() const { return DebuffDuration;}
	void SetDebuffDuration(float bInDebuffDuration) { DebuffDuration = bInDebuffDuration;}
	
	float GetDebuffFrequency() const { return DebuffFrequency;}
	void SetDebuffFrequency(float bInDebuffFrequency) { DebuffFrequency = bInDebuffFrequency;}

	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType;}
	void SetDamageType(const TSharedPtr<FGameplayTag>& bInDamageType) { DamageType =  bInDamageType;}

	/*Add Impulse and Force*/
	FVector GetDeathImpulse() const { return DeathImpulse;}
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse;}

	FVector GetKnockbackForce() const { return KnockbackForce;}
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce;}
	


	/*Radial Damage*/
	bool GetIsRadialDamage() const { return bIsRadialDamage;}
	void SetIsRadialDamage(bool InbIsRadialDamage) { bIsRadialDamage = InbIsRadialDamage;}
	
	float GeRadialDamageInnerRadius() const { return RadialDamageInnerRadius;}
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius;}

	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius;}
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius;}

	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin;}
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin;}
	
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

	UPROPERTY()
	bool bIsRadialDamage = false;
	
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
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