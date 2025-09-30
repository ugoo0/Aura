// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"

FDamageEffectParams UAuraDamageGameplayAbility::GetDamageEffectParamsForTarget(AActor* Target, FVector RadialOrigin) const
{
	FDamageEffectParams Params;
	Params.WorldContext = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.BaseDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	Params.DamageType = DamageType;
	Params.AbilityLevel = GetAbilityLevel();

	Params.DebuffChance = DebuffChance;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	if (IsValid(Target))
	{
		Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);//一开始没有目标的情况 在造成伤害的时候需要手动设置
		FRotator Rotation = (Target->GetActorLocation()-GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;//造成伤害的时候设置
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;//造成伤害的时候设置
	}

	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = RadialOrigin;

		FRotator Rotation = (Target->GetActorLocation()-RadialOrigin).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToRadialOrigin = Rotation.Vector();
		Params.KnockbackForce = ToRadialOrigin * KnockbackForceMagnitude;
		Params.DeathImpulse = ToRadialOrigin * DeathImpulseMagnitude;
	}
	return  Params;
}

FDamageEffectParams UAuraDamageGameplayAbility::GetDamageEffectParamsForTargetOverride(AActor* Target,
	bool InIsRadialDamage, FVector InRadialOrigin,float InRadialDamageOuterRadius, float InRadialDamageInnerRadius, FVector InDeathImpulse, FVector InKnockbackForce) const
{
	FDamageEffectParams Params;
	Params.WorldContext = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);//一开始没有目标的情况 在造成伤害的时候需要手动设置
	Params.BaseDamage = DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	Params.DamageType = DamageType;
	Params.AbilityLevel = GetAbilityLevel();

	Params.DebuffChance = DebuffChance;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.bIsRadialDamage = bIsRadialDamage;
	Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
	Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	
	if (IsValid(Target))
	{
		FRotator Rotation = (Target->GetActorLocation()-GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;//造成伤害的时候设置
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;//造成伤害的时候设置
	}

	if (InIsRadialDamage)
	{
		Params.bIsRadialDamage = InIsRadialDamage;
		Params.RadialDamageOrigin = InRadialOrigin;
		if (InRadialDamageOuterRadius)
		{
			Params.RadialDamageOuterRadius = InRadialDamageOuterRadius;
		}
		else
		{
			Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		}
		if (InRadialDamageInnerRadius)
		{
			Params.RadialDamageInnerRadius = InRadialDamageInnerRadius;
			
		}
		else
		{
			Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		}
		
		
		
		FRotator Rotation = (Target->GetActorLocation()-InRadialOrigin).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToRadialOrigin = Rotation.Vector();
		Params.KnockbackForce = ToRadialOrigin * KnockbackForceMagnitude;
		Params.DeathImpulse = ToRadialOrigin * DeathImpulseMagnitude;
	}

	if (!InDeathImpulse.IsZero())
	{
		Params.DeathImpulse = InDeathImpulse * DeathImpulseMagnitude;
	}

	if (!InKnockbackForce.IsZero())
	{
		Params.KnockbackForce = InKnockbackForce * KnockbackForceMagnitude;
	}

	
	return  Params;
}

void UAuraDamageGameplayAbility::CauseDamageToTarget(AActor* Target, const TArray<FName>& TagsGetHurt)
{
	bool bHasTag = false;
	for (auto Tag : TagsGetHurt)
	{
		if (Target->ActorHasTag(Tag)) bHasTag = true;
	}
	if (!bHasTag) return;
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
	float ScaleDamage =  DamageScalableFloat.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,DamageType,ScaleDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
}

float UAuraDamageGameplayAbility::GetDamageByLevel(int32 Level) const
{
	return DamageScalableFloat.GetValueAtLevel(Level);
}