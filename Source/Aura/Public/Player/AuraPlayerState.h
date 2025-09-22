// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraPlayerState.generated.h"

class ULevelInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32/*Level or XP*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChanged, int32/*CurLevel*/, int32/*NewLevel*/)


class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	virtual int32 GetPlayerLevel_Implementation() override;

	FORCEINLINE void AddToLevel(int32 InLevel);
	FORCEINLINE void SetLevel(int32 InLevel);
	
	FORCEINLINE void AddToXP(int32 InXP);
	FORCEINLINE void SetXP(int32 InXP);
	FORCEINLINE int32 GetXP() const;

	virtual void Die() override;

	FOnPlayerStateChanged OnPlayerXPChangedDelegate;
	FOnPlayerLevelChanged OnPlayerLevelChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category="GAS|LevelInfo")
	TObjectPtr<ULevelInfo> AuraLevelInfo;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
