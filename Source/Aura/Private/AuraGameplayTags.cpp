// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::AuraGameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/* PrimaryAttributesTags begin */
	AuraGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"),FString("增加物理伤害"));
	AuraGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"),FString("增加魔法伤害"));
	AuraGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"),FString("增加护甲和护甲穿透"));
	AuraGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"),FString("增加恢复和血量"));
	/* PrimaryAttributesTags end */
	

	/* SecondaryAttributesTags begin */
	AuraGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"),FString("护甲属性:增加伤害，增加闪避概率"));
	AuraGameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"),FString("护甲穿透：无视对面部分护甲，提高暴击概率"));
	AuraGameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"),FString("闪避概率：概率减少受到的伤害"));
	AuraGameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"),FString("暴击概率:概率提高单次伤害"));
	AuraGameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"),FString("暴击伤害：暴击时提高的伤害"));
	AuraGameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"),FString("暴击抗性：减少受暴击概率"));
	AuraGameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"),FString("每秒恢复的血量"));
	AuraGameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"),FString("每秒恢复的蓝量"));
	AuraGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"),FString("最大生命值"));
	AuraGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"),FString("最大蓝量"));

	AuraGameplayTags.Attributes_Secondary_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Resistance.Fire"),FString("火焰伤害抗性"));
	AuraGameplayTags.Attributes_Secondary_Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Resistance.Lighting"),FString("光属性伤害抗性"));
	AuraGameplayTags.Attributes_Secondary_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Resistance.Arcane"),FString("魔法伤害抗性"));
	AuraGameplayTags.Attributes_Secondary_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Resistance.Physical"),FString("物理伤害抗性"));


	/* SecondaryAttributesTags end */

	/*begin AbilityInputTags*/
	AuraGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),FString("鼠标左键触发的技能"));
	AuraGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),FString("鼠标右键触发的技能"));
	AuraGameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),FString("按键1触发的技能"));
	AuraGameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),FString("按键2触发的技能"));
	AuraGameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),FString("按键3触发的技能"));
	AuraGameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),FString("按键4触发的技能"));
	
	/*end AbilityInputTags*/

	/* Ability Tag */
	AuraGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("能力里面传递伤害的Tag"));

	AuraGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),FString("能力里面传递火焰伤害的Tag"));
	AuraGameplayTags.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"),FString("能力里面传递闪电伤害的Tag"));
	AuraGameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),FString("能力里面传递魔法伤害的Tag"));
	AuraGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),FString("能力里面传递物理伤害的Tag"));
	
	/* Effect Tag */
	AuraGameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"),FString("对造成伤害反应的Tag"));

	/*Abilities Tags */
	AuraGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("普通攻击Tag"));
	AuraGameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("召唤随处Tag"));
	AuraGameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("火球术Tag"));

	/*Abilities Cooldown Tags */
	AuraGameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("火球术冷却Tag"));

	/*Montage Tag*/
	AuraGameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("使用武器攻击"));
	AuraGameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("使用左手攻击"));
	AuraGameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("使用右手攻击"));
	AuraGameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("使用尾巴攻击"));
	

	AuraGameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage发生的攻击事件1"));
	AuraGameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage发生的攻击事件2"));
	AuraGameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage发生的攻击事件3"));
	AuraGameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage发生的攻击事件4"));
	
	AuraGameplayTags.AttributesTag.Reset();
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Primary_Intelligence);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Primary_Strength);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Primary_Resilience);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Primary_Vigor);

	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_MaxHealth);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_MaxMana);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_Armor);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_ArmorPenetration);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_BlockChance);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitChance);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitDamage);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitResistance);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_HealthRegeneration);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_ManaRegeneration);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Fire);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Lighting);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Arcane);
	AuraGameplayTags.AttributesTag.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Physical);

	AuraGameplayTags.DamageTypeToResistance.Add(AuraGameplayTags.Damage_Fire, AuraGameplayTags.Attributes_Secondary_Resistance_Fire);
	AuraGameplayTags.DamageTypeToResistance.Add(AuraGameplayTags.Damage_Lighting, AuraGameplayTags.Attributes_Secondary_Resistance_Lighting);
	AuraGameplayTags.DamageTypeToResistance.Add(AuraGameplayTags.Damage_Arcane, AuraGameplayTags.Attributes_Secondary_Resistance_Arcane);
	AuraGameplayTags.DamageTypeToResistance.Add(AuraGameplayTags.Damage_Physical, AuraGameplayTags.Attributes_Secondary_Resistance_Physical);
}
