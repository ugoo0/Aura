// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> Result;
	for (const FLootItem& Item : LootItems)
	{
		for (int i = 0; i < Item.MaxNumToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem LootItem;
				LootItem.ItemClass = Item.ItemClass;
				LootItem.bLootLevelOverride = Item.bLootLevelOverride;
				Result.Add(LootItem);
			}
		}
	}
	return Result;
}
