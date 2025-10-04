// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapName == InMapName) return true;
	}
	return  false;
}

FSavedMap ULoadScreenSaveGame::GetSavedMap(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapName == InMapName) return SavedMap;
	}
	return FSavedMap();
}
