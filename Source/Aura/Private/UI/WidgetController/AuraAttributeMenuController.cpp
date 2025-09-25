// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraAttributeMenuController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void UAuraAttributeMenuController::BroadcastInitialValues()
{
	check(GetAuraAS());
	for (const auto& data : GetAuraAS()->TagToAttribute)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
		Info.Value = data.Value().GetNumericValue(GetAuraAS());
		AttributeInfoDelegate.Broadcast(Info);
	}
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAuraAttributeMenuController::BindCallbackToDependencies()
{
	check(GetAuraAS());
	for (const auto& data : GetAuraAS()->TagToAttribute)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
		Info.Value = data.Value().GetNumericValue(GetAuraAS());
		AttributeInfoDelegate.Broadcast(Info);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(data.Value()).AddLambda(
			[this,&data](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
				Info.Value = data.Value().GetNumericValue(GetAuraAS());
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
}

void UAuraAttributeMenuController::UpgradeAttribute(const FGameplayTag& GameplayTag)
{
	GetAuraASC()->UpgradeAttribute(GameplayTag);
}
