// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
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

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float TimeRemoveAfterDie;
	virtual void Die() override;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting =false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 200.f;

	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charcter Class Default")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charcter Class Default")
	ECharacterClassType CharacterClassType = ECharacterClassType::Warrior;
	
	UPROPERTY(BlueprintAssignable,Category = "UserInterface|HealthBar")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category = "UserInterface|HealthBar")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserInterface|HealthBar")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	virtual void InitializaDefaultAttriutes() const override;

	void HitReactTagChanged(const FGameplayTag Tag, int32 NewTagCount);

	virtual void Destroyed() override;
};
