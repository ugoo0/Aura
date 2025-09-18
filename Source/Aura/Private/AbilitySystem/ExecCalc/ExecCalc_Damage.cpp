// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"


struct AuraDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLighting);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical);

	AuraDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ResistanceFire,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ResistanceLighting,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ResistanceArcane,Target,false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ResistancePhysical,Target,false)

	}
	
	const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& GetTagsToDefs() const 
	{
		if (!bInitialized)
		{
			FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_Armor,ArmorDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_BlockChance,BlockChanceDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_ArmorPenetration,ArmorPenetrationDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitChance,CriticalHitChanceDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitDamage,CriticalHitDamageDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_CriticalHitResistance,CriticalHitResistanceDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Fire,ResistanceFireDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Lighting,ResistanceLightingDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Arcane,ResistanceArcaneDef);
			TagsToDefs.Add(AuraGameplayTags.Attributes_Secondary_Resistance_Physical,ResistancePhysicalDef);
			bInitialized = true;
		}
		return TagsToDefs;
	}
private:
	mutable TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToDefs;
	mutable bool bInitialized = false;
};

static const AuraDamageStatic& DamageStatic()
{
	static AuraDamageStatic DStatic;
	return DStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ResistanceFireDef);
	RelevantAttributesToCapture.Add(DamageStatic().ResistanceLightingDef);
	RelevantAttributesToCapture.Add(DamageStatic().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(DamageStatic().ResistancePhysicalDef);	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	
	
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;
	UCharacterClassInfo* ClassDefaultInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	float Damage = 0.f;

	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToDefs = DamageStatic().GetTagsToDefs();
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypeToResistance)
	{
		FGameplayTag DamageType = Pair.Key;
		FGameplayTag ResistanceType = Pair.Value;
		float TargetDamageTypeResist = 0.f;
		checkf(TagsToDefs.Contains(ResistanceType), TEXT("TagsToDefs 不包含对应伤害类型的Tag: %s"),*ResistanceType.ToString());
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToDefs[ResistanceType],EvalParams,TargetDamageTypeResist);
		TargetDamageTypeResist = FMath::Clamp(TargetDamageTypeResist,0.f,100.f);
		float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType,false) * (100.f-TargetDamageTypeResist)/100.f;
		
		Damage += TypeDamage;
	}
	
	//格挡
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().BlockChanceDef,EvalParams,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance,0.f);
	const bool bBlocked = FMath::RandRange(1,100) <= TargetBlockChance;
	Damage = bBlocked ? Damage * 0.5f : Damage;

	//暴击和暴击伤害
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitResistanceDef,EvalParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance,0.f);

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitChanceDef,EvalParams,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance,0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitDamageDef,EvalParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage,0.f);

	const FRealCurve* CriticalHitResistanceCurve = ClassDefaultInfo->DamageCalculationCoeffients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	const bool bCriticalHit = FMath::RandRange(1,100) <= (SourceCriticalHitChance-TargetCriticalHitResistance * CriticalHitResistanceCoefficient);
	
	Damage  = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage : Damage;
	
	//护甲和护甲穿透
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmorDef,EvalParams,TargetArmor);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance,0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmorPenetrationDef,EvalParams,SourceArmorPenetration);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance,0.f);


	const FRealCurve* ArmorPenetrationCurve = ClassDefaultInfo->DamageCalculationCoeffients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	EffectiveArmor = FMath::Max<float>(EffectiveArmor,0.f);

	const FRealCurve* ArmorCurve = ClassDefaultInfo->DamageCalculationCoeffients->FindCurve(FName("Armor"), FString());
	const float ArmorCoefficient = ArmorPenetrationCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	Damage *= (100.f - EffectiveArmor * ArmorCoefficient) / 100.f;


	UAuraAbilitySystemLibrary::SetIsBlockHit(EffectContextHandle,bBlocked);
	UAuraAbilitySystemLibrary::SetIsCritical(EffectContextHandle,bCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
