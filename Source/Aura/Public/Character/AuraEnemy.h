// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighLightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"


class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighLightInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	/** IHighLightInterface **/
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& Destination) override;
	/** IHighLightInterface **/
	

	/** CombatInterface **/
	virtual int32 GetPlayerLevel_Implementation() override;
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
	virtual void Die(const FVector& DeathImpulse) override;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting =false;




	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	AActor* CombatTarget;
	
	
	virtual  void SetCombatTarget_Implementation(AActor* Target) override;
	
	virtual AActor* GetCombatTarget_Implementation() override;

	virtual  void OnStunTagCountChanged(const FGameplayTag GameplayTag, int32 Count) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charcter Class Default")
	int32 Level = 1;
	
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
