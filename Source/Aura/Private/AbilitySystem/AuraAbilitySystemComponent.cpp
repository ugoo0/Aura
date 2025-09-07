// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AblityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilitys)
{
	for (auto AbilityClass : StartupAbilitys)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupTag);
			GiveAbility(AbilitySpec);
		}
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	TArray<FGameplayAbilitySpec> AbilitySpecs =  GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	TArray<FGameplayAbilitySpec> AbilitySpecs =  GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
	UE_LOG(LogTemp, Warning, TEXT("EffectApplied"));
}
