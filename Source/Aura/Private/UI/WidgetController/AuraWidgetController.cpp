// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

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

FWidgetControllerParams::FWidgetControllerParams()
{
}
