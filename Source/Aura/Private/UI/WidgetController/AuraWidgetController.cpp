// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetParams)
{
	PlayerController = WidgetParams.PlayerController;
	PlayerState = WidgetParams.PlayerState;
	AbilitySystemComponent = WidgetParams.AbilitySystemComponent;
	AttributeSet = WidgetParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbackToDependencies()
{
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPS == nullptr)
	{
		AuraPS =  Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPS;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAS == nullptr)
	{
		AuraAS =  Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAS;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC =  Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraASC;
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPC == nullptr)
	{
		AuraPC =  Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPC;
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;
	FForEachAbility Delegate;
	Delegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfos->FindAbilityInfoForAbilityTag(GetAuraASC()->GetAbilityTagForAbilitySpec(AbilitySpec));
		Info.InputTag = GetAuraASC()->GetInputTagForAbilitySpec(AbilitySpec);
		Info.StatusTag = GetAuraASC()->GetStatusTagForAbilitySpec(AbilitySpec);
		FAbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraASC()->ForEachAbility(Delegate);
}

FWidgetControllerParams::FWidgetControllerParams()
{
}
