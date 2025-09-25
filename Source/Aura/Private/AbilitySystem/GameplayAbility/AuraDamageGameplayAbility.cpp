// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamageToTarget(AActor* Target, const TArray<FName>& TagsGetHurt)
{
	bool bHasTag = false;
	for (auto Tag : TagsGetHurt)
	{
		if (Target->ActorHasTag(Tag)) bHasTag = true;
	}
	if (!bHasTag) return;
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	for (const TTuple<FGameplayTag, FScalableFloat>& pair : DamageTypes)
	{
		float ScaleDamage =  pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,pair.Key,ScaleDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
}

int32 UAuraDamageGameplayAbility::GetDamageByLevelAndDamageType(int32 Level, const FGameplayTag& DamageType) const
{
	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbility[%s] dose not contain DamageType[%s]"), *GetNameSafe(this), DamageType.GetTagName())
	return DamageTypes[DamageType].GetValueAtLevel(Level);
}