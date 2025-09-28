// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilitySystemLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageStateStruct.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraInterface.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerController.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	/* PrimaryAttributes */
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Primary_Strength,GetStrengthAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Primary_Resilience,GetResilienceAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Primary_Vigor,GetVigorAttribute);

	/* SecondaryAttributes */
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_Armor,GetArmorAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxMana,GetManaAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_Resistance_Fire,GetResistanceFireAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_Resistance_Lighting,GetResistanceLightingAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_Resistance_Arcane,GetResistanceArcaneAttribute);
	TagToAttribute.Add(FAuraGameplayTags::Get().Attributes_Secondary_Resistance_Physical,GetResistancePhysicalAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/*
		Vital Attributes
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	/*
		Primary Attributes
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	/*
		Secondary Attributes
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceLighting, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = NewValue < 0 ? 0 : NewValue;
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = NewValue < 0 ? 0 : NewValue;
	}
	
}

void UAuraAttributeSet::SetFEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const
{
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	EffectProperties.SourceAsc = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(EffectProperties.SourceAsc) && EffectProperties.SourceAsc->AbilityActorInfo.IsValid() && EffectProperties.SourceAsc->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceAsc->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceController = EffectProperties.SourceAsc->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		if (EffectProperties.SourceController != nullptr)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAsc = &Data.Target;
		EffectProperties.TargetAvatarActor = EffectProperties.TargetAsc->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = EffectProperties.TargetAsc->AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAsc->AbilityActorInfo->AvatarActor.Get());
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)//升级回满体和蓝 
{
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProperties)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		const bool bFatal = NewHealth <=  0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
			if (CombatInterface)
			{
				FVector DeathImpulse = UAuraAbilitySystemLibrary::GetDeathImpulse(EffectProperties.EffectContextHandle);
				CombatInterface->Die(DeathImpulse);
			}
			//获取经验
			SendXPRewardEvent(EffectProperties);
		}
		else
		{
			if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_GetIsBeingShock(EffectProperties.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);
				EffectProperties.TargetAsc->TryActivateAbilitiesByTag(TagContainer);
			}


			const FVector KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(EffectProperties.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				EffectProperties.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		ShowFloatingText(LocalIncomingDamage, EffectProperties);
		if (UAuraAbilitySystemLibrary::GetIsSuccessfulDebuff(EffectProperties.EffectContextHandle))
		{
			HandleDebuff(EffectProperties);
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& EffectProperties)
{
	const int32 LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	if (EffectProperties.SourceCharacter->Implements<UAuraInterface>())
	{

		int32 CurLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.SourceCharacter);
		int32 CurXP = IAuraInterface::Execute_GetXP(EffectProperties.SourceCharacter);

		int32 NewXP = CurXP + LocalIncomingXP;
		int32 NewLevel = IAuraInterface::Execute_FindLevelForExperience(EffectProperties.SourceCharacter,NewXP);

		int32 LevelUpCount = NewLevel - CurLevel;
		IAuraInterface::Execute_AddToXP(EffectProperties.SourceCharacter,LocalIncomingXP);
		if (LevelUpCount > 0)
		{
			// const int32 AttributePointsReward = IAuraInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter, CurLevel, NewLevel);
			// const int32 SpellPointsReward = IAuraInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter, CurLevel, NewLevel);
			IAuraInterface::Execute_LevelUp(EffectProperties.SourceCharacter,CurLevel, NewLevel);
			bTopOffHealth = true;
			bTopOffMana = true;
		}
	}
}

void UAuraAttributeSet::HandleDebuff(const FEffectProperties& EffectProperties)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle DebuffEffectContext = EffectProperties.SourceAsc->MakeEffectContext();
	DebuffEffectContext.AddSourceObject(EffectProperties.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDebuffDamageType(EffectProperties.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(EffectProperties.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(EffectProperties.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(EffectProperties.EffectContextHandle);
	const FGameplayTag DebuffType = GameplayTags.DamageTypeToDebuffType[DamageType];

	FString DebuffName = FString::Printf(TEXT("Debuff_%s"), *DebuffType.ToString());

	UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
	DebuffEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DebuffEffect->DurationMagnitude = FScalableFloat(DebuffDuration);
	DebuffEffect->Period = FScalableFloat(DebuffFrequency);

	DebuffEffect->InheritableOwnedTagsContainer.AddTag(DebuffType);

	DebuffEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	DebuffEffect->StackLimitCount = 1;

	const int32 Index = DebuffEffect->Modifiers.Num();
	DebuffEffect->Modifiers.Add(FGameplayModifierInfo());
	
	FGameplayModifierInfo& DebuffModifier = DebuffEffect->Modifiers[Index];
	DebuffModifier.Attribute = GetIncomingDamageAttribute();
	DebuffModifier.ModifierOp = EGameplayModOp::Additive;
	DebuffModifier.ModifierMagnitude = FScalableFloat(DebuffDamage);
	
	if (FGameplayEffectSpec* DebuffSpec = new FGameplayEffectSpec(DebuffEffect, DebuffEffectContext, 1.f))
	{
		FAuraGameplayEffectContext* DebuffAuraContext = static_cast<FAuraGameplayEffectContext*>(DebuffEffectContext.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		DebuffAuraContext->SetDamageType(DebuffDamageType);

		EffectProperties.TargetAsc->ApplyGameplayEffectSpecToSelf(*DebuffSpec);
	}
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties EffectProperties;
	SetFEffectProperties(Data, EffectProperties);

	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter)) return;
	
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())//受伤害逻辑
	{
		HandleIncomingDamage(EffectProperties);
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())//SourceCharacter是玩家
	{
		HandleIncomingXP(EffectProperties);
	}
	
}

void UAuraAttributeSet::ShowFloatingText(float Damage,const FEffectProperties& EffectProperties) const
{

	
	if (EffectProperties.SourceCharacter)//玩家攻击敌人
	{
		if (AAuraPlayerController* PC= Cast<AAuraPlayerController>(EffectProperties.SourceCharacter->Controller))
		{
			FDamageState DamageState;
			DamageState.IsBlockHit = UAuraAbilitySystemLibrary::IsBlockHit(EffectProperties.EffectContextHandle);
			DamageState.IsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
			PC->ShowDamageNumber(Damage,EffectProperties.TargetCharacter,DamageState);
			return;
		}
	}
	if (AAuraPlayerController* PC= Cast<AAuraPlayerController>(EffectProperties.TargetCharacter->Controller))//Enemy攻击玩家
	{
		FDamageState DamageState;
		DamageState.IsBlockHit = UAuraAbilitySystemLibrary::IsBlockHit(EffectProperties.EffectContextHandle);
		DamageState.IsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
		PC->ShowDamageNumber(Damage,EffectProperties.TargetCharacter,DamageState);
	}
}

void UAuraAttributeSet::SendXPRewardEvent(const FEffectProperties& EffectProperties)
{
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		ECharacterClassType TargetClassType = ICombatInterface::Execute_GetCharacterClassType(EffectProperties.TargetCharacter);
		int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.TargetCharacter);
		int32 XPReward = UAuraAbilitySystemLibrary::GetRewardXPForCharacterClassAndLevel(EffectProperties.TargetCharacter,TargetClassType, TargetLevel);
		FGameplayEventData Payload;
		Payload.EventTag = FAuraGameplayTags::Get().Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, Payload.EventTag, Payload);
	}

}

/*
	Vital Attributes
*/

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);

}

/*
	Primary Attributes
*/

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);

}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);

}

/*
	Secondary Attributes
*/

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);

}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);

}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);

}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}


void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceFire, OldResistanceFire);
}

void UAuraAttributeSet::OnRep_ResistanceLighting(const FGameplayAttributeData& OldResistanceLighting) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceLighting, OldResistanceLighting);
}

void UAuraAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceArcane, OldResistanceArcane);
}

void UAuraAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistancePhysical, OldResistancePhysical);
}
