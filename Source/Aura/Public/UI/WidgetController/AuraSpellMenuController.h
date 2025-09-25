// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuController.generated.h"


struct FAuraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAbilitySpellGlobeSelected, bool, EnabledButtonSpendPoint, bool,EnabledButtonEquipped, FString, Description, FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCheckPlayWaitingEquippedAnim, const FGameplayTag&, AbilityType, bool, WaitingEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAfterAbilityEquipped);

struct FSelectedSpellTags
{
public:
	FGameplayTag AbilityTag;
	FGameplayTag AbilityStatusTag;
	FGameplayTag AbilityTypeTag;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraSpellMenuController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override; //
	virtual void BindCallbackToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilitySpellGlobeSelected OnAbilitySpellGlobeSelected;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedSignature SpellPointsChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FCheckPlayWaitingEquippedAnim CheckPlayWaitingEquippedAnim;
	UPROPERTY(BlueprintAssignable)
	FAfterAbilityEquipped AfterAbilityEquipped;
	
	UFUNCTION(BlueprintCallable)
	void SpellButtonSelectedForAbilityTag(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendSpellPoints();

	UFUNCTION(BlueprintCallable)
	void DeselectSpellButton();

	UFUNCTION(BlueprintCallable)
	void OnClickEquippedButton();

	UFUNCTION(BlueprintCallable)
	void OnClickRowEquippedGlobe(const FGameplayTag& AbilityType, const FGameplayTag& SlotTag);
	
	bool bWaitingEquipped = false;

	FGameplayTag SelectedSlot;
private:
	void ShouldEnableSpellButton(const FGameplayTag& AbilityStatusTag, bool& bEnableSpendPoints, bool& bEnableEquipped);
	FSelectedSpellTags SelectedSpellTags = {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_None};
};
