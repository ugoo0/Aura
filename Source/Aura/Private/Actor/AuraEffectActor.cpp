// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include <AbilitySystemInterface.h>
#include <AbilitySystem/AuraAttributeSet.h>
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GamePlayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return;
	check(GamePlayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GamePlayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	bool IsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (IsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlay(AActor* Target)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(Target, InstantGamePlayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(Target, DurationGamePlayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(Target, InfiniteGamePlayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlay(AActor* Target)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(Target, InstantGamePlayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(Target, DurationGamePlayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(Target, InfiniteGamePlayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!IsValid(TargetASC)) return;
		TArray<FActiveGameplayEffectHandle> HandleToRemove;
		for (auto& Handle : ActiveEffectHandles)
		{
			if (Handle.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(Handle.Key, 1);//默认移除所有stacks
				HandleToRemove.Add(Handle.Key);
			}
		}
		for (auto& Handle : HandleToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

// Called every frame
void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

