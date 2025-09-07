// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AS);
	for (const auto& data : AS->TagToAttribute)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
		Info.Value = data.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAuraMenuWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AS);
	for (const auto& data : AS->TagToAttribute)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
		Info.Value = data.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(data.Value()).AddLambda(
			[this,&data,AS](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(data.Key);
				Info.Value = data.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}

}
