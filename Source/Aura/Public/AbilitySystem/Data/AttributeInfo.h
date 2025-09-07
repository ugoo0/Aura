// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeDesc = FText();

	UPROPERTY(BlueprintReadOnly)
	float Value = 0.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	FAuraAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
