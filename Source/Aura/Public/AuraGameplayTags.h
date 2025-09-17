// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FAuraGameplayTags
{
public:
	static  FAuraGameplayTags& Get()
	{
		return AuraGameplayTags;
	}
	static  void InitializeNativeGameplayTags();
	/*begin Attributes Tag*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Secondary_Resistance_Fire;
	FGameplayTag Attributes_Secondary_Resistance_Lighting;
	FGameplayTag Attributes_Secondary_Resistance_Arcane;
	FGameplayTag Attributes_Secondary_Resistance_Physical;
	
	/*end Attributes Tag*/
	
	/*begin AbilityInputTags*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	/*end AbilityInputTags*/

	/*Ability Damage Tag*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	/* Ability Tag */
	FGameplayTag Abilities_Attack;
	/*Effect*/
	FGameplayTag Effect_HitReact;

	/*Montage Tag*/
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	
	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistance; // 用于计算伤害的时候找对应抗性
	TSet<FGameplayTag> AttributesTag; //用于在Attributes Menu显示逻辑
private:
	static FAuraGameplayTags AuraGameplayTags;
};
