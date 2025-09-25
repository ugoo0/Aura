// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/AuraInterface.h"

void UAuraAbilitySystemComponent::AblityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities)
{
	for (auto AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities)
{
	for (auto AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);//直接激活被动
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoints_Implementation(const FGameplayTag& AbilityTag)
{
	FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	if (AbilityTag.IsValid() && !AbilityTag.MatchesTagExact(AuraGameplayTags.Abilities_None))
	{
		FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag);
		FGameplayTag AbilityStatusTag = GetStatusTagForAbilitySpec(*Spec);
		if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Eligible))//解锁能力
		{
			Spec->DynamicAbilityTags.RemoveTag(AbilityStatusTag);
			Spec->DynamicAbilityTags.AddTag(AuraGameplayTags.Abilities_Status_Unlocked);
			AbilityStatusTag = AuraGameplayTags.Abilities_Status_Unlocked;
		}
		else if (AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Unlocked) || AbilityStatusTag.MatchesTagExact(AuraGameplayTags.Abilities_Status_Equipped))
		{
			Spec->Level += 1;
		}
		if (GetAvatarActor()->Implements<UAuraInterface>())
		{
			IAuraInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		MarkAbilitySpecDirty(*Spec);
		ClientAbilityStatusChanged(AbilityTag, AbilityStatusTag, Spec->Level);
	}
}

void UAuraAbilitySystemComponent::ServerEquippedAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag)
{
	//GetActivatableAbilities() 究极大雷函数  FGameplayAbilitySpec& Spec 和 auto  Spec 调用的是不同版本的函数
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())//移除要装载位置的  输入tag和改变status Tag
		{
			if (Spec.DynamicAbilityTags.HasTagExact(SlotTag))
			{
				
				Spec.DynamicAbilityTags.RemoveTag(SlotTag);
				if (Spec.DynamicAbilityTags.HasTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
				{
					Spec.DynamicAbilityTags.RemoveTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
					Spec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Unlocked);
				}
				MarkAbilitySpecDirty(Spec);
			}
		}


		if (PreSlotTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))//之前没装载过
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(FAuraGameplayTags::Get().Abilities_Status_Unlocked);//修改status
			AbilitySpec->DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
		}
		else
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(PreSlotTag);//移除之前的inputtag
		}
		AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);
		
		MarkAbilitySpecDirty(*AbilitySpec);
		ClientEquippedAbility(AbilityTag,PreSlotTag, SlotTag);
	}
}

void UAuraAbilitySystemComponent::ClientEquippedAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& PreSlotTag, const FGameplayTag& SlotTag)
{
	AbilitiesEquipped.Broadcast(AbilityTag, PreSlotTag, SlotTag);
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& GameplayTag, FString& OutDescription,
                                                             FString& OutNextLevelDescription)
{
	if (!GameplayTag.IsValid() || GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
		OutNextLevelDescription = FString();
		return  false;
	}
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(GameplayTag))
	{
		UAuraGameplayAbility* Ability = Cast<UAuraGameplayAbility>(AbilitySpec->Ability.Get());
		OutDescription = Ability->GetDescription(AbilitySpec->Level);
		OutNextLevelDescription = Ability->GetNextLevelDescription(AbilitySpec->Level + 1);
		return  true;
	}
	FAuraAbilityInfo AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor())->FindAbilityInfoForAbilityTag(GameplayTag);
	OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo.LevelRequirement);
	OutNextLevelDescription = FString();
	return  false;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()//客户端同步
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}



void UAuraAbilitySystemComponent::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	TArray<FGameplayAbilitySpec> AbilitySpecs =  GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	TArray<FGameplayAbilitySpec> AbilitySpecs =  GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : AbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}



FGameplayTag UAuraAbilitySystemComponent::GetMatchTagForAbilitySpecAndFName(const FGameplayAbilitySpec& AbilitySpec,
                                                                            FName TagMatchName)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(TagMatchName)))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}


FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const
{
	return GetMatchTagForAbilitySpecAndFName(AbilitySpec, FName("Abilities"));
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const
{
	if (!AbilitySpec.DynamicAbilityTags.IsEmpty())
	{
		for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag")))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec) const
{
	if (!AbilitySpec.DynamicAbilityTags.IsEmpty())
	{
		for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities.Status")))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagForInputTag(const FGameplayTag& InputTag) const
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return GetAbilityTagForAbilitySpec(AbilitySpec);
		}
	}

	return  FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagForInputTag(const FGameplayTag& InputTag) const
{
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return GetStatusTagForAbilitySpec(AbilitySpec);
		}
	}

	return  FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& GameplayTag)
{
	if (GetAvatarActor()->Implements<UAuraInterface>())
	{
		if (IAuraInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(GameplayTag);
		}
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& GameplayTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability.Get()->AbilityTags.HasTagExact(GameplayTag)) return &Spec;
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::ServerUpdateAbilityStatus_Implementation(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (FAuraAbilityInfo Info : AbilityInfo->AbilityInfos)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			GEngine->AddOnScreenDebugMessage(1,10.f,FColor::Red,FString("GiveAbilityGiveAbility"));
			MarkAbilitySpecDirty(AbilitySpec);
			ClientAbilityStatusChanged(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, AbilitySpec.Level);
		}
	}
}

void UAuraAbilitySystemComponent::ClientAbilityStatusChanged_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, AbilityStatusTag,AbilityLevel);
}

// void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
// {
// 	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
// 	for (FAuraAbilityInfo Info : AbilityInfo->AbilityInfos)
// 	{
// 		if (!Info.AbilityTag.IsValid()) continue;
// 		if (Level < Info.LevelRequirement) continue;
// 		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
// 		{
// 			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
// 			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
// 			GiveAbility(AbilitySpec);
// 			GEngine->AddOnScreenDebugMessage(1,10.f,FColor::Red,FString("GiveAbilityGiveAbility"));
// 			MarkAbilitySpecDirty(AbilitySpec);
// 			ClientAbilityStatusChanged(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, AbilitySpec.Level);
// 		}
// 	}
// }

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& GameplayTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = GameplayTag;
	Payload.EventMagnitude = 1;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), Payload.EventTag, Payload);
	if (GetAvatarActor()->Implements<UAuraInterface>())
	{
		IAuraInterface::Execute_AddToAttributePoints(GetAvatarActor(),-1);
	}
}


void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
