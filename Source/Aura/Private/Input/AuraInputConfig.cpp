// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByGameplayTag(const FGameplayTag& GameplayTag,
	bool bLogNotFound) const
{
	for (auto Action:AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == GameplayTag)
		{
				return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for tag %s"), *GameplayTag.ToString());
	}
	return nullptr;
}
