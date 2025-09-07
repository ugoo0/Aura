// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEFectAssetTags,const FGameplayTagContainer&)

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

	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilitys);

	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
protected:

	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};
