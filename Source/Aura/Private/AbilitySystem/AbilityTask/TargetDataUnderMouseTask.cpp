// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouseTask.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouseTask* UTargetDataUnderMouseTask::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseTask* MyObject = NewAbilityTask<UTargetDataUnderMouseTask>(OwningAbility);
	return MyObject;
}

void UTargetDataUnderMouseTask::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),GetActivationPredictionKey()).AddUObject(this,&UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(),GetActivationPredictionKey());
		GEngine->AddOnScreenDebugMessage(1,5.f,FColor::Red,FString("bCalledDelegatebCalledDelegate"));
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouseTask::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	FHitResult CursorHit;
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouseTask::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
