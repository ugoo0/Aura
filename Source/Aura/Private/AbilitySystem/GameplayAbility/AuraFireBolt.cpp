// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraFireBolt.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	
	
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc;
	if (Level == 1)
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>FireBolt</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
			//Desc
			"<Default>Launches a fire bolt, exploding on impact and dealing :</>"
			"<Damage>%d</>" //Damage
			"<Default>fire Damage with a chance to burn</>\n"
			
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
			"<Title>FireBolt</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
			//Desc
			"<Default>Launches %d fire bolts, exploding on impact and dealing :</>"
			"<Damage>%d</>" //Damage
			"<Default>fire Damage with a chance to burn</>\n"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, Level),
			Damage);
		}	
	return Desc;
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc = FString::Printf(TEXT(
		//Title
		"<Title>FireBolt</>\n\n "
		//Level
		"<Small>Level : </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost : </><ManaCost>%d</>\n"
		//Cooldown
		"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
		//Desc
		"<Default>Launches %d fire bolts, exploding on impact and dealing :</>"
		"<Damage>%d</>" //Damage
		"<Default>fire Damage with a chance to burn</>\n"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(NumProjectiles, Level),
		Damage);
	return Desc;
}




