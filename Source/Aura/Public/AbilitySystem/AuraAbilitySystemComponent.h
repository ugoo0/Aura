// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEFectAssetTags,const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);

/**
 * 
 */
class UGameplayAbility;

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AblityActorInfoSet();

	FEFectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;

	bool bStartupAbilitiesGiven = false;

	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilitys);

	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);
	static FGameplayTag GetAbilityTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
protected:

    virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};
