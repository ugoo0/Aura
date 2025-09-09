// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAuraMenuWidgetController* UAuraAbilitySystemLibrary::GetMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClassType Type, float level,
	UAbilitySystemComponent* ASC)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;
		FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterClassDefaultInfoForType(Type);
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(ASC->GetAvatarActor());
		FGameplayEffectSpecHandle PrimaryAttributeEffectSpec = ASC->MakeOutgoingSpec(DefaultInfo.PrimaryAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeEffectSpec.Data.Get());

		FGameplayEffectSpecHandle SecondaryAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeEffectSpec.Data.Get());
		
		FGameplayEffectSpecHandle VitalAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeEffectSpec.Data.Get());
	}
}
