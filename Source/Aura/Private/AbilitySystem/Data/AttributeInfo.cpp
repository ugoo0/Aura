// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"


FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for ( const FAuraAttributeInfo&  AttributeInfo : AttributeInformation)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("FAuraAttributeInfo::FindAttributeInfoByTag: Cannot find AttributeInfo for %s"), *AttributeTag.GetTagName().ToString());
	}
	return FAuraAttributeInfo();
}
