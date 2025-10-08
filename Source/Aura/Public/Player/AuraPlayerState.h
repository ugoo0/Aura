// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraPlayerState.generated.h"

class ULevelInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32/*Level or XP*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnPlayerLevelChanged, int32/*CurLevel*/, int32/*NewLevel*/, bool/*upLevel*/)


class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	/* GAS */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
	
	/* 需要网络复制的属性时需要override这个函数 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/* Getter Setter */
	int32 GetPlayerLevel() const {return Level;}
	void SetPlayerLevel(int32 InLevel);
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);
	void AddToAttributePoints(int32 Points);
	void AddToSpellPoints(int32 Points);
	
	FORCEINLINE int32 GetXP() const;
	FORCEINLINE int32 GetAttributePoints() const {return  AttributePoints;}
	FORCEINLINE int32 GetSpellPoints() const {return SpellPoints;}
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
	
	FOnPlayerStateChanged OnPlayerXPChangedDelegate;
	FOnPlayerLevelChanged OnPlayerLevelChangedDelegate;
	FOnPlayerStateChanged OnAttributePointsChangedDelegate;
	FOnPlayerStateChanged OnSpellPointsChangedDelegate;

	/* 每一个等级包含的信息 所需经验，奖励技能和属性点 */
	UPROPERTY(EditDefaultsOnly, Category="GAS|LevelInfo")
	TObjectPtr<ULevelInfo> AuraLevelInfo;
protected:
	/* GAS相关 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
private:
	/* 玩家基础信息 需要网络同步*/
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
    int32 AttributePoints = 0;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
    int32 SpellPoints = 5;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldPoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldPoints);
};
