// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include <GameplayEffect.h>
#include <UI/Widget/AuraUserWidget.h>

#include "AbilitySystem/Data/AbilityInfo.h"
#include "OverlayWidgetController.generated.h"


class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);//创建委托，事件， 类型为FOnAttributeChangedSignature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChangedSignature, int32, CurLevel, int32, NewLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, FAuraAbilityInfo, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	//属性
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	//XP
	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnPlayerXPPercentChanged;
	//Level
	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnPlayerLevelChangedSignature OnPlayerLevelChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FMessageWidgetRowSignature FMessageWidgetRowDelegate;
	

	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
	FAbilityInfoSignature FAbilityInfoDelegate;
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC);

	void OnPlayerXPChanged(int32 InXP);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfos;
	
template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
