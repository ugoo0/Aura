// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"


class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	/** EnemyInterface **/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** EnemyInterface **/
	

	/** CombatInterface **/
	FORCEINLINE virtual int32 GetPlayerLevel() const override;
	/** CombatInterface **/
	
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(EditAnywhere)
	float testNum;

	UPROPERTY(EditAnywhere)
	float testNum2;

	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charcter Class Default")
	int32 Level = 1;

	UPROPERTY(BlueprintAssignable,Category = "UserInterface|HealthBar")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category = "UserInterface|HealthBar")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserInterface|HealthBar")
	TObjectPtr<UWidgetComponent> HealthBarWidget;
};
