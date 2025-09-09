// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfoForType(const ECharacterClassType& Type)
{
	return  CharacterClassInformation.FindChecked(Type);
}
