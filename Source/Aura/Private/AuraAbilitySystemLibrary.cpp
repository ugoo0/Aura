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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes_SetByCaller(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* LoadData)
{
	if (AAuraGameModeBase* GameMode =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
		UCharacterClassInfo* ClassInfo = GameMode->CharacterClassInfo;
		FGameplayEffectContextHandle PrimaryAttributesEffectContext = ASC->MakeEffectContext();
		PrimaryAttributesEffectContext.AddSourceObject(ASC->GetAvatarActor());
		FGameplayEffectSpecHandle PrimaryAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->PrimaryAttributes_SetByCaller,1.0f,PrimaryAttributesEffectContext);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpec,AuraGameplayTags.Attributes_Primary_Strength,LoadData->Strength);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpec,AuraGameplayTags.Attributes_Primary_Intelligence,LoadData->Intelligence);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpec,AuraGameplayTags.Attributes_Primary_Resilience,LoadData->Resilience);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttributeEffectSpec,AuraGameplayTags.Attributes_Primary_Vigor,LoadData->Vigor);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeEffectSpec.Data.Get());


		FGameplayEffectContextHandle SecondaryAttributeEffectContext = ASC->MakeEffectContext();
		SecondaryAttributeEffectContext.AddSourceObject(ASC->GetAvatarActor());
		FGameplayEffectSpecHandle SecondaryAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes_Infinite,1.0f,SecondaryAttributeEffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeEffectSpec.Data.Get());
		

		FGameplayEffectContextHandle VitalAttributeEffectContext = ASC->MakeEffectContext();
		VitalAttributeEffectContext.AddSourceObject(ASC->GetAvatarActor());
		FGameplayEffectSpecHandle  VitalAttributeEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes,1.0f,VitalAttributeEffectContext);
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

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis,
	float Spread, int32 SpreadNum)
{
	TArray<FVector> Result;
	if (SpreadNum > 1)
	{
		float SpreadDelta = Spread/(SpreadNum - 1);
		FVector RightVector = Forward.RotateAngleAxis(-Spread/2, Axis);
		for (int32 i = 0; i < SpreadNum; i++)
		{
			FVector Direction = RightVector.RotateAngleAxis(i * SpreadDelta, Axis);
			Result.Add(Direction);
		}
	}
	else
	{
		Result.Add(Forward);
	}
	return Result;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlyRotatedRotators(const FVector& Forward, const FVector& Axis,
	float Spread, int32 SpreadNum)
{
	TArray<FRotator> Result;
	if (SpreadNum > 1)
	{
		float SpreadDelta = Spread/(SpreadNum - 1);
		FVector RightVector = Forward.RotateAngleAxis(-Spread/2, Axis);
		for (int32 i = 0; i < SpreadNum; i++)
		{
			FVector Direction = RightVector.RotateAngleAxis(i * SpreadDelta, Axis);
			Result.Add(Direction.Rotation());
		}
	}
	else
	{
		Result.Add(Forward.Rotation());
	}
	return Result;
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
	/*radial Damage*/
	SetIsRadialDamage(EffectContextHandle,Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle,Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle,Params.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle,Params.RadialDamageOrigin);
	
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

bool UAuraAbilitySystemLibrary::GetIsRadialDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetIsRadialDamage();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& ContextHandle, bool InIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsRadialDamage(InIsRadialDamage);
	}
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GeRadialDamageInnerRadius();
	}
	return 0.f;
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& ContextHandle,
	float InRadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& ContextHandle,
	float InRadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InRadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
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
}

bool UAuraAbilitySystemLibrary::IsFriend(const AActor* Actor1, const AActor* Actor2)
{
	bool Actor1IsPlayer = Actor1->ActorHasTag("Player");
	bool Actor2IsPlayer = Actor2->ActorHasTag("Player");
	bool Actor1IsEnemy = Actor1->ActorHasTag("Enemy");
	bool Actor2IsEnemy = Actor2->ActorHasTag("Enemy");
	return (Actor1IsPlayer && Actor2IsPlayer) || (Actor1IsEnemy && Actor2IsEnemy);
}


