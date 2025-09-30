// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc;
	if (Level == 1)
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>Electrocute</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
			//Desc
			"<Default>Emits a beam of lighting,</>"
			"<Default>Cause </>"
			"<Damage>%d</>" //Damage
			"<Default> Lightning Damage with a chance to Stun.</>\n"
			
			),
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>Electrocute</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
			//Desc
			"<Default>Emits a beam of lighting, "
			"propagating to %d additional targets nearby, causing </>"
			"<Damage>%d</>" //Damage
			"<Default> lighting Damage with a chance to stun.</>\n"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(MaxBeamSpreadNum, Level),
			Damage);
	}	
	return Desc;
}

FString UAuraElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc = FString::Printf(TEXT(
	//Title
	"<Title>Electrocute</>\n\n"

	//Level
	"<Small>Level : </><Level>%d</>\n"
	//ManaCost
	"<Small>ManaCost : </><ManaCost>%d</>\n"
	//Cooldown
	"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
	//Desc
	"<Default>Emits a beam of lighting, "
	"propagating to %d additional targets nearby, causing </>"
	"<Damage>%d</>" //Damage
	"<Default> lighting Damage with a chance to stun.</>\n"
	),
	Level,
	ManaCost,
	Cooldown,
	FMath::Min(MaxBeamSpreadNum, Level),
	Damage);
	return Desc;
}
