// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <AbilitySystem/AuraAbilitySystemComponent.h>

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()//绑定函数到属性变化会分发的事件，一旦属性变化会调用所绑定的lambda函数，函数里继续分发UI事件
{
	GetAuraPS()->OnPlayerXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerXPChanged);
	GetAuraPS()->OnPlayerLevelChangedDelegate.AddLambda(
		[this](int32 CurLevel, int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(CurLevel, NewLevel);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	
	if (GetAuraASC())
	{
		if (GetAuraASC()->bStartupAbilitiesGiven)//还没绑定委托就被广播了
		{
			BroadcastAbilityInfo();
		}
		else//还没广播
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& TagContainer)
			{
				for (FGameplayTag Tag : TagContainer)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						FMessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

	GetAuraASC()->AbilitiesEquipped.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag)
	{

		//Reset SpellEquippedGlobe
		FAuraAbilityInfo Info;
		Info.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
		Info.InputTag = PreSlotTag;
		FAbilityInfoDelegate.Broadcast(Info);
		BroadcastAbilityInfo();
	});

}


void UOverlayWidgetController::OnPlayerXPChanged(int32 InXP)
{
	OnPlayerXPPercentChanged.Broadcast(GetAuraPS()->AuraLevelInfo->GetShowPercentForExperience(InXP));
}



