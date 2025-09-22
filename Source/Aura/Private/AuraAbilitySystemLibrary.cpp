// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
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

UAuraMenuWidgetController* UAuraAbilitySystemLibrary::GetMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return HUD->GetMenuWidgetController(WidgetControllerParams);
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

int32 UAuraAbilitySystemLibrary::GetRewardXPForCharacterClassAndLevel(const UObject* WorldContextObject,
	ECharacterClassType CharacterClassType, int32 Level)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!ClassInfo) return 0;
	FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterClassDefaultInfoForType(CharacterClassType);
	return static_cast<int32> (DefaultInfo.XPReward.GetValueAtLevel(Level));
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


