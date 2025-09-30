// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc;
	if (Level == 1)
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>ArcaneShards</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
			//Desc
			"<Default>Summon shards of arcane energy, </>"
			"cause radial damage of "
			"<Damage>%d</>" //Damage
			"<Default> at the shard origin.</>\n"
			
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
			"<Title>ArcaneShards</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
			//Desc
			"<Default>Summon %d shards of arcane energy, causing </>"
			"radial arcane damage of "
			"<Damage>%d</>" //Damage
			"<Default> at the shard origins.</>\n"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(MaXNumShards, Level),
			Damage);
	}	
	return Desc;
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc = FString::Printf(TEXT(
	//Title
	"<Title>ArcaneShards</>\n\n"

	//Level
	"<Small>Level : </><Level>%d</>\n"
	//ManaCost
	"<Small>ManaCost : </><ManaCost>%d</>\n"
	//Cooldown
	"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
	//Desc
	"<Default>Summon %d shards of arcane energy, causing </>"
	"radial arcane damage of "
	"<Damage>%d</>" //Damage
	"<Default> at the shard origins.</>\n"
	),
	Level,
	ManaCost,
	Cooldown,
	FMath::Min(MaXNumShards, Level),
	Damage);
	return Desc;
}
