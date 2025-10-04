// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
	
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffType, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTypeCountChanged);
	}
	else
	{
		if (CombatInterface)
		{
			CombatInterface->GetASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* NewASC)
			{
				NewASC->RegisterGameplayTagEvent(DebuffType, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::OnDebuffTypeCountChanged);
			});
		}
	}

	if (CombatInterface)
	{
		CombatInterface->GetActorDeadDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDead);
	}
}

void UDebuffNiagaraComponent::OnDebuffTypeCountChanged(const FGameplayTag GameplayTag, int32 Count)
{
	bool bOwnerDeath = ICombatInterface::Execute_IsDead(GetOwner());
	if (Count > 0 && !bOwnerDeath)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDead(AActor* Actor)
{
	Deactivate();
}
