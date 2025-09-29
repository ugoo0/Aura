// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/PassiveSpell/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())) ;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (ASC)
	{
		ASC->OnPassiveStateChanged.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveAbilityStateChanged);
	}
	else
	{
		if (CombatInterface)
		{
			CombatInterface->GetASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* NewASC)
			{
				
				if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(NewASC))
				{
					AuraASC->OnPassiveStateChanged.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveAbilityStateChanged);
				}
			});
		}
	}
}

void UPassiveNiagaraComponent::OnPassiveAbilityStateChanged(const FGameplayTag& AbilityTag, bool bPassiveActivated)
{
	if (AbilityTag.MatchesTagExact(PassiveTag))
	{
		if (bPassiveActivated)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
