// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/SplineComponent.h"
#include "AuraPlayerController.generated.h"


class UNiagaraSystem;
class UDamageWidgetComponent;
struct FGameplayTag;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY() 
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float Damage, ACharacter* Target,const FDamageState& DamageState) const;

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	bool bShiftPressed = false;
	void ShiftInputPressed(){bShiftPressed = true;}
	void ShiftInputReleased(){bShiftPressed = false;}
	
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* CurActor;
private:

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent>  AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FHitResult CursorHit;
	/*移动相关*/
	FVector CashedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;
	
	void AutoRun();

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<UDamageWidgetComponent> DamageTextComponentClass;

	UFUNCTION()
	bool CharacterIsControlled() const;//玩家状态是否被控制 比如 眩晕

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	UFUNCTION()
	void UpdateMagicCirclePosition();
};


