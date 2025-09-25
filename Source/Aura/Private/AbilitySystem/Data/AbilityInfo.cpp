// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForAbilityTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("AbilityInfo::FindAbilityIngoForAbilityTag - AbilityTag %s not found"), *AbilityTag.GetTagName().ToString());
	}
	return  FAuraAbilityInfo();
}
