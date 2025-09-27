// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEFectAssetTags,const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged,const FGameplayTag&/*Ability Tag*/, const FGameplayTag /*Ability StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilitiesEquipped,const FGameplayTag&/*Ability Tag*/, const FGameplayTag /*PreSlotTag*/, FGameplayTag /*SlotTag*/);

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
	FAbilitiesEquipped AbilitiesEquipped;
	bool bStartupAbilitiesGiven = false;

	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities);
	void AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities);

	
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);
	FGameplayTag GetAbilityTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const;
	FGameplayTag GetInputTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const;
	FGameplayTag GetStatusTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const;
	FGameplayTag GetAbilityTagForInputTag(const FGameplayTag& InputTag) const;
	FGameplayTag GetStatusTagForInputTag(const FGameplayTag& InputTag) const;
	
	void UpgradeAttribute(const FGameplayTag& GameplayTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& GameplayTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& GameplayTag);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAbilityStatus(int32 Level);

	FAbilityStatusChanged AbilityStatusChanged;

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoints(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquippedAbility(const FGameplayTag& AbilityTag, const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag);

	UFUNCTION(Client,Reliable)
	void ClientEquippedAbility(const FGameplayTag& AbilityTag, const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag);
	
	bool GetDescriptionByAbilityTag(const FGameplayTag& GameplayTag, FString& OutDescription, FString& OutNextLevelDescription);

	
protected:

    virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(Client,Reliable)
	void ClientAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, int32 AbilityLevel);
private:
	static FGameplayTag GetMatchTagForAbilitySpecAndFName(const FGameplayAbilitySpec& AbilitySpec, FName TagMatchName);
};
