// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Data/AttributeInfo.h"
#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAttributeMenuController.generated.h"

struct FAuraAttributeInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraAttributeMenuController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS:Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS:AttributePoints")
	FOnPlayerStateChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable, Category="GAS:AttributePoints")
	void UpgradeAttribute(const FGameplayTag& GameplayTag);
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAttributeInfo>  AttributeInfo;
};
