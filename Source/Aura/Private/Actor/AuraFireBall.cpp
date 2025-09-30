// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraFireBall.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AAuraFireBall::AAuraFireBall()
{
	
}

void AAuraFireBall::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CheckIsValidOverlay(OtherActor)) return;
	if (HasAuthority())
	{
		if (!bOverlay)
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.TargetASC = TargetASC;
				DamageEffectParams.DeathImpulse = DeathImpulse;
				
				UAuraAbilitySystemLibrary::ApplyDamageEffectForDamageEffectParms(DamageEffectParams.TargetASC->GetAvatarActor() ,DamageEffectParams);
			}
		
		}
	}
	else
	{
		bOverlay = true;
	}
}

void AAuraFireBall::BroadCastFinalExplodeDelegateToTargets(TArray<AActor*> Targets, bool IsRadialDamage,
	FVector RadialOrigin)
{
	for (AActor* Target : Targets)
	{
		FDamageEffectParams Params;
		Params.bIsRadialDamage = IsRadialDamage;
		Params.RadialDamageOrigin = RadialOrigin;


		FRotator Rotation = (Target->GetActorLocation()-RadialOrigin).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToRadialOrigin = Rotation.Vector();
		Params.KnockbackForce = ToRadialOrigin;
		Params.DeathImpulse = ToRadialOrigin;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		
		OnFinalExplodeDelegate.Broadcast(Target, Params);
	}
}

void AAuraFireBall::OnHit()
{
	if (IsValid(GetOwner()))
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
		LoopingSoundComp->DestroyComponent();
	}
	bOverlay = true;
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartTimeLine();
}
