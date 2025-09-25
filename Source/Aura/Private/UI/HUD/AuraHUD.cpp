// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AuraSpellMenuController.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& FWCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(FWCParams);
		OverlayWidgetController->BindCallbackToDependencies();
	}
	return OverlayWidgetController;
}

UAuraAttributeMenuController* AAuraHUD::GetAttributeMenuController(const FWidgetControllerParams& FWCParams)
{
	if (AttributeMenuController == nullptr)
	{
		AttributeMenuController = NewObject<UAuraAttributeMenuController>(this, AttributeMenuControllerClass);
		AttributeMenuController->SetWidgetControllerParams(FWCParams);
		AttributeMenuController->BindCallbackToDependencies();
	}
	return AttributeMenuController;
}

UAuraSpellMenuController* AAuraHUD::GetSpellMenuController(const FWidgetControllerParams& FWCParams)
{
	if (SpellMenuController == nullptr)
	{
		SpellMenuController = NewObject<UAuraSpellMenuController>(this, SpellMenuControllerClass);
		SpellMenuController->SetWidgetControllerParams(FWCParams);
		SpellMenuController->BindCallbackToDependencies();
	}
	return SpellMenuController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Uninitialized, please fill out BP_AuraHUD"));
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
