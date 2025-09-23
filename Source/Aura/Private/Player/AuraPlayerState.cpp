// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
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
	
}

int32 AAuraPlayerState::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	int32 CurLevel = Level;
	Level += InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(CurLevel, Level);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	int32 CurLevel = Level;
	Level = InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(CurLevel, Level);
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
	OnSpellPointsDelegate.Broadcast(SpellPoints);
}

int32 AAuraPlayerState::GetXP() const
{
	return  XP;
}

void AAuraPlayerState::Die()
{
	
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	int32 CurLevel = Level;
	OnPlayerLevelChangedDelegate.Broadcast(CurLevel, OldLevel);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnPlayerXPChangedDelegate.Broadcast(OldXP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 Points)
{
	OnAttributePointsChangedDelegate.Broadcast(Points);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 Points)
{
	OnSpellPointsDelegate.Broadcast(Points);
}
