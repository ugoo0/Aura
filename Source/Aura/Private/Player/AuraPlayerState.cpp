// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/* ReplicationMode:: Minimal Mixed Full */
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
}

void AAuraPlayerState::SetPlayerLevel(int32 InLevel)
{
	int32 OldLevel = Level;
	Level = InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(OldLevel, Level, false);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	int32 CurLevel = Level;
	Level += InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(CurLevel, Level, true);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	int32 CurLevel = Level;
	Level = InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(CurLevel, Level, false);
}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToAttributePoints(int32 Points)
{
	AttributePoints += Points;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 Points)
{
	SpellPoints += Points;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

int32 AAuraPlayerState::GetXP() const
{
	return  XP;
}

void AAuraPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	SpellPoints = InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(OldLevel, Level, true);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnPlayerXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldPoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
