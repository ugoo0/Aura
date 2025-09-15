// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext :  public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	bool GetIsCriticalHit() const { return bIsCriticalHit;}
	bool GetIsBlockHit() const { return bIsBlockHit;}
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit;}
	void SetIsBlockHit(bool bInIsBlockHit) { bIsBlockHit = bInIsBlockHit;}
	virtual FAuraGameplayEffectContext* Duplicate() const override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
protected:

private:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockHit = false;
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