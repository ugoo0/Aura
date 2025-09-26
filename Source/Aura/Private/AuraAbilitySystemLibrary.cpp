// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAuraAttributeMenuController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetAttributeMenuController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAuraSpellMenuController* UAuraAbilitySystemLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetSpellMenuController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClassType Type, float level,
                                                            UAbilitySystemComponent* ASC)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;
		FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterClassDefaultInfoForType(Type);
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(ASC->GetAvatarActor());
		FGameplayEffectSpecHandle PrimaryAttributeEffectSpec = ASC->MakeOutgoingSpec(DefaultInfo.PrimaryAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeEffectSpec.Data.Get());

		FGameplayEffectSpecHandle SecondaryAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeEffectSpec.Data.Get());
		
		FGameplayEffectSpecHandle VitalAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes,level,EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeEffectSpec.Data.Get());
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClassType CharacterClassType)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;
		if (!ClassInfo) return;;
		for (auto Ability : ClassInfo->CommonAbility)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,1);	
			ASC->GiveAbility(AbilitySpec);
		}
		FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterClassDefaultInfoForType(CharacterClassType);
		int32 Level = 1;
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			Level = ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor());
		}
		for (auto Ability : DefaultInfo.StartupAbilities)
		{

			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,Level);	
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;
		return ClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UAbilityInfo* AbilityInfo = GameMode->AbilityInfo;
		return AbilityInfo;
	}
	return nullptr;
}

int32 UAuraAbilitySystemLibrary::GetRewardXPForCharacterClassAndLevel(const UObject* WorldContextObject,
                                                                      ECharacterClassType CharacterClassType, int32 Level)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!ClassInfo) return 0;
	FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterClassDefaultInfoForType(CharacterClassType);
	return static_cast<int32> (DefaultInfo.XPReward.GetValueAtLevel(Level));
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffectForDamageEffectParms(const UObject* WorldContextObject,
	const FDamageEffectParams& Params)
{
	FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();
	FGameplayEffectContextHandle EffectContextHandle = Params.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	if (FMath::RandRange(1,100) < Params.KnockbackChance)
	{
		SetKnockbackForce(EffectContextHandle, Params.KnockbackForce);
	}
	FGameplayEffectSpecHandle SpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageEffectClass,Params.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Params.DamageType,Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,AuraGameplayTags.Debuff_Chance,Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,AuraGameplayTags.Debuff_Damage,Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,AuraGameplayTags.Debuff_Frequency,Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,AuraGameplayTags.Debuff_Duration,Params.DebuffDuration);
	
	Params.SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Params.TargetASC);
	return EffectContextHandle;
}

bool UAuraAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsBlockHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::GetIsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDebuffDuration();
	}
	return 0.f;
}
 FGameplayTag UAuraAbilitySystemLibrary::GetDebuffDamageType(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		if (AuraGameplayEffectContext->GetDamageType().IsValid())
		{
			return *AuraGameplayEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetKnockbackForce();
	}

	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& ContextHandle, bool bBlockHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlockHit(bBlockHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCritical(FGameplayEffectContextHandle& ContextHandle, bool bIsCritical)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(bIsCritical);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& ContextHandle,
	bool bIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsSuccessfulDebuff(bIsSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& ContextHandle, float InDebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& ContextHandle, float InDebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& ContextHandle, float InDebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamageType(FGameplayEffectContextHandle& ContextHandle,
	const FGameplayTag& DebuffDamageType)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> SharedPtr = MakeShared<FGameplayTag>(DebuffDamageType);
		AuraGameplayEffectContext->SetDamageType(SharedPtr);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}



void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InKnockbackForce)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlayActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin, float Radius)
{
	OutOverlayActors.Reset();
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);
		for (const FOverlapResult& OverlapResult : Overlaps)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlayActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(OverlapResult.GetActor()));
			}
		}
	}
	TArray<FOverlapResult> a1111;
}

bool UAuraAbilitySystemLibrary::IsFriend(const AActor* Actor1, const AActor* Actor2)
{
	bool Actor1IsPlayer = Actor1->ActorHasTag("Player");
	bool Actor2IsPlayer = Actor2->ActorHasTag("Player");
	bool Actor1IsEnemy = Actor1->ActorHasTag("Enemy");
	bool Actor2IsEnemy = Actor2->ActorHasTag("Enemy");
	return (Actor1IsPlayer && Actor2IsPlayer) || (Actor1IsEnemy && Actor2IsEnemy);
}


