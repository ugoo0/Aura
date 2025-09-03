// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSoket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{

}

void AAuraCharacterBase::ApplyGameEffectByGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffect, float Level = 1.0f) const
{
	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponent();
	if (TargetASC == nullptr) return;
	check(GameplayEffect);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect, Level, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
}

void AAuraCharacterBase::InitalizePrimaryAttributes() const
{
	ApplyGameEffectByGameplayEffectClass(DefaultPrimaryAttributesGameEffectClass);
}

void AAuraCharacterBase::InitalizeSecondaryAttributes() const
{
	ApplyGameEffectByGameplayEffectClass(DefaultSecondaryAttributesGameEffectClass);
}

void AAuraCharacterBase::InitalizeVitalAttributes() const
{
	ApplyGameEffectByGameplayEffectClass(DefaultVitalAttributesGameEffectClass);
}



