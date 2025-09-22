// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::FindLevelForExperience(int32 Experience)
{
	if (Levels.IsEmpty()) return 1;
	Levels.Sort([](const FAuraLevelInfo& A, const FAuraLevelInfo& B)
	{
		return A.LevelUpRequirement < B.LevelUpRequirement;
	});
	for (int32 i = 0; i < Levels.Num(); i++)
	{
		if (Experience < Levels[i].LevelUpRequirement)
		{
			return Levels[i].Level;
		}
	}
	return  Levels[Levels.Num()-1].Level;
}

float ULevelInfo::GetShowPercentForExperience(int32 Experience)
{
	int32 level = FindLevelForExperience(Experience);
	int32 CurLevelRequirement = GetLevelInfoForLevel(level).LevelUpRequirement;
	int32 LastLevelRequirement =level > 1 ? GetLevelInfoForLevel(level-1).LevelUpRequirement : 0;
	return static_cast<float>(Experience-LastLevelRequirement)/static_cast<float>(CurLevelRequirement-LastLevelRequirement);
}

FAuraLevelInfo ULevelInfo::GetLevelInfoForLevel(int32 InLevel)
{
	for (auto LevelInfo : Levels)
	{
		if (LevelInfo.Level == InLevel)
		{
			return LevelInfo;
		}
	}
	return FAuraLevelInfo();
}
