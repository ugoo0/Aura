// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* ASC,
	const FGameplayTag& InCoolDownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = ASC;
	WaitCooldownChange->CooldownTag = InCoolDownTag;
	if (!IsValid(ASC) || !InCoolDownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return  nullptr;
	}

	WaitCooldownChange->ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);
	
	
	WaitCooldownChange->ASC->RegisterGameplayTagEvent(InCoolDownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &UWaitCooldownChange::CoolDownTagChange);


	

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CoolDownTagChange(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllAssetTags(AssetTags);
	EffectSpec.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery  = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> RemainTimes = AbilitySystemComponent->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (RemainTimes.Num() > 0)
		{
			float RemainTime = 0.f;
			for (auto time : RemainTimes)
			{
				if (RemainTime < time) RemainTime = time;
			}
			CooldownStart.Broadcast(RemainTime);
		}
	}
}
