// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "View/MVVMView.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	MVVM_LoadScreen = NewObject<UMVVM_LoadScreen>(this, MVVM_LoadScreenClass);
	MVVM_LoadScreen->InitialLoadSlots();
	
	LoadScreenOverlay = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenOverlayClass);
	LoadScreenOverlay->AddToViewport();
	LoadScreenOverlay->BlueprintInitializeWidget();
	
}
