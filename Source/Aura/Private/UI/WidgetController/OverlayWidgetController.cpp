// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <AbilitySystem/AuraAbilitySystemComponent.h>

#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()//绑定函数到属性变化会分发的事件，一旦属性变化会调用所绑定的lambda函数，函数里继续分发UI事件
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnPlayerXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerXPChanged);

	AuraPlayerState->OnPlayerLevelChangedDelegate.AddLambda(
		[this](int32 CurLevel, int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(CurLevel, NewLevel);
		});
	
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (AuraASC)
	{
		if (AuraASC->bStartupAbilitiesGiven)//还没绑定委托就被广播了
		{
			OnInitializeStartupAbilities(AuraASC);
		}
		else//还没广播
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}
		Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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

}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	if (!AuraASC->bStartupAbilitiesGiven) return;
	FForEachAbility Delegate;
	Delegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfos->FindAbilityIngoForAbilityTag(AuraASC->GetAbilityTagForAbilitySpec(AbilitySpec));
		Info.InputTag = AuraASC->GetInputTagForAbilitySpec(AbilitySpec);
		FAbilityInfoDelegate.Broadcast(Info);
	});
	AuraASC->ForEachAbility(Delegate);
}

void UOverlayWidgetController::OnPlayerXPChanged(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnPlayerXPPercentChanged.Broadcast(AuraPlayerState->AuraLevelInfo->GetShowPercentForExperience(InXP));
}



