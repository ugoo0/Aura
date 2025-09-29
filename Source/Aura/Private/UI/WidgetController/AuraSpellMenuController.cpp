// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuController.h"

#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void UAuraSpellMenuController::BindCallbackToDependencies()
{

	//技能树技能状态发生变化时调用
	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, int32 AbilityLevel)
		{
			auto Info =	AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag);
			Info.StatusTag = AbilityStatusTag;
			FAbilityInfoDelegate.Broadcast(Info);

			if (AbilityTag.MatchesTagExact(SelectedSpellTags.AbilityTag))
			{
				SelectedSpellTags.AbilityStatusTag = AbilityStatusTag;
				bool bEnableSpendPoints;
				bool bEnableEquipped;
				ShouldEnableSpellButton(SelectedSpellTags.AbilityStatusTag, bEnableSpendPoints,bEnableEquipped);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionByAbilityTag(SelectedSpellTags.AbilityTag, Description, NextLevelDescription, AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag).LevelRequirement);
				OnAbilitySpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
			}
		});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointsChangedDelegate.Broadcast(SpellPoints);

			bool bEnableSpendPoints;
			bool bEnableEquipped;
			ShouldEnableSpellButton(SelectedSpellTags.AbilityStatusTag, bEnableSpendPoints,bEnableEquipped);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionByAbilityTag(SelectedSpellTags.AbilityTag, Description, NextLevelDescription, AbilityInfos->FindAbilityInfoForAbilityTag(SelectedSpellTags.AbilityStatusTag).LevelRequirement);
			OnAbilitySpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
		});

		GetAuraASC()->AbilitiesEquipped.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag)
		{
			bWaitingEquipped = false;
			CheckPlayWaitingEquippedAnim.Broadcast(AbilityTag, bWaitingEquipped);
			AfterAbilityEquipped.Broadcast();			
			//Reset SpellEquippedGlobe
			FAuraAbilityInfo Info;
			Info.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
			Info.InputTag = PreSlotTag;
			FAbilityInfoDelegate.Broadcast(Info);

			//set SpellEquippedGlobe
			FAuraAbilityInfo InfoNew = AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag);
			InfoNew.InputTag = SlotTag;
			FAbilityInfoDelegate.Broadcast(InfoNew);
			
			// BroadcastAbilityInfo();
		});
}

void UAuraSpellMenuController::SpellButtonSelectedForAbilityTag(const FGameplayTag& AbilityTag)
{
	if (bWaitingEquipped)
	{
		bWaitingEquipped = false;
		CheckPlayWaitingEquippedAnim.Broadcast(AbilityTag, bWaitingEquipped);
	}
	
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	FGameplayTag AbilityStatusTag;
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(AuraGameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	if (bTagNone || !bTagValid || AbilitySpec == nullptr)
	{
		AbilityStatusTag = AuraGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatusTag = GetAuraASC()->GetStatusTagForAbilitySpec(*AbilitySpec);
	}
	bool bEnableSpendPoints;
	bool bEnableEquipped;
	SelectedSpellTags.AbilityTag = AbilityTag;
	SelectedSpellTags.AbilityStatusTag = AbilityStatusTag;
	SelectedSpellTags.AbilityTypeTag = AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag).AbilityType;
	ShouldEnableSpellButton(AbilityStatusTag, bEnableSpendPoints,bEnableEquipped);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(SelectedSpellTags.AbilityTag, Description, NextLevelDescription, AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag).LevelRequirement);
	OnAbilitySpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquipped, Description, NextLevelDescription);
}

void UAuraSpellMenuController::SpendSpellPoints()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoints(SelectedSpellTags.AbilityTag);
	}
}

void UAuraSpellMenuController::DeselectSpellButton()
{
	if (bWaitingEquipped)
	{
		bWaitingEquipped = false;
		CheckPlayWaitingEquippedAnim.Broadcast(SelectedSpellTags.AbilityTypeTag, bWaitingEquipped);
	}
	
	SelectedSpellTags.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	// SelectedSpellTags.AbilityTypeTag = FAuraGameplayTags::Get().Abilities_None;
	// SelectedSpellTags.AbilityStatusTag = FAuraGameplayTags::Get().Abilities_None;
	OnAbilitySpellGlobeSelected.Broadcast(false, false, FString(), FString());
	
}

void UAuraSpellMenuController::OnClickEquippedButton()
{
	bWaitingEquipped = true;
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(SelectedSpellTags.AbilityTag, Description, NextLevelDescription,  AbilityInfos->FindAbilityInfoForAbilityTag(SelectedSpellTags.AbilityStatusTag).LevelRequirement);
	OnAbilitySpellGlobeSelected.Broadcast(false, false, Description, NextLevelDescription);
	CheckPlayWaitingEquippedAnim.Broadcast(SelectedSpellTags.AbilityTypeTag, bWaitingEquipped);

	SelectedSlot = FAuraGameplayTags::Get().Abilities_None;
	if (SelectedSpellTags.AbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetInputTagForAbilitySpec(*GetAuraASC()->GetSpecFromAbilityTag(SelectedSpellTags.AbilityTag));
	}
}

void UAuraSpellMenuController::OnClickRowEquippedGlobe(const FGameplayTag& AbilityType, const FGameplayTag& SlotTag)
{
	if (!bWaitingEquipped) return;//还没点击装载
	if (!AbilityType.MatchesTagExact(SelectedSpellTags.AbilityTypeTag)) return;//类型不一致
	if (SelectedSlot.MatchesTagExact(SlotTag)) return;//之前的位置和要装载的位置一样
	GetAuraASC()->ServerEquippedAbility(SelectedSpellTags.AbilityTag, SelectedSlot, SlotTag);
}



void UAuraSpellMenuController::ShouldEnableSpellButton(const FGameplayTag& AbilityStatusTag, bool& bEnableSpendPoints,
                                                       bool& bEnableEquipped)
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	bEnableSpendPoints = false;
	bEnableEquipped = false;

	if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Locked))
	{

	}
	else if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bEnableSpendPoints = true;
		}
	}
	else if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked))
	{
		if (SpellPoints > 0)
		{
			bEnableSpendPoints = true;
		}
		bEnableEquipped = true;
	}
	else if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
	{
		if (SpellPoints > 0)
		{
			bEnableSpendPoints = true;
		}
		bEnableEquipped = true;
	}
	
}
