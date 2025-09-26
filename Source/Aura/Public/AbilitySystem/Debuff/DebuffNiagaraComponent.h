// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffType;

	UDebuffNiagaraComponent();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDebuffTypeCountChanged(const FGameplayTag GameplayTag, int32 Count);

	UFUNCTION()
	void OnOwnerDead(AActor* Actor);
};
