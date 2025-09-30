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
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
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
		FRotator Rotation = (Target->GetActorLocation()-GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
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