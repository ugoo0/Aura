// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>

void UOverlayWidgetController::BroadcastInitialValues()
{

	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
}

