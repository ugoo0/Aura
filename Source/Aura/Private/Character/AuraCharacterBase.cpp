// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* AAuraCharacterBase::GetMelleAttackMontage_Implementation()
{
	return MelleAttackMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath();
	bDead = true;
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(MeshDissolvedMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMeshMetarialInst = UMaterialInstanceDynamic::Create(MeshDissolvedMaterialInstance,this);
		GetMesh()->SetMaterial(0,DynamicMeshMetarialInst);
		StartDissolveMeshTimeline(DynamicMeshMetarialInst);
	}
	if (IsValid(WeaponDissolvedMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicWeaponMetarialInst = UMaterialInstanceDynamic::Create(WeaponDissolvedMaterialInstance,this);
		Weapon->SetMaterial(0,DynamicWeaponMetarialInst);
		StartDissolveWeaponTimeline(DynamicWeaponMetarialInst);
	}
}

UNiagaraSystem* AAuraCharacterBase::GetHitNiagaraSystem_Implementation() const
{
	return BloodEffect;
}

FTagsToMontage AAuraCharacterBase::GetTagsToMontageByMontageTag_Implementation(const FGameplayTag& MontageTag)
{
	for (const FTagsToMontage& TagToMontage : TagsToMontages)
	{
		if (TagToMontage.MontageGameplayTag.MatchesTagExact(MontageTag))
		{
			return TagToMontage;
		}
	}
	return FTagsToMontage();
}

int32 AAuraCharacterBase::GetMaxMinionCount_Implementation()
{
	return MaxMinionCount;
}

void AAuraCharacterBase::AddMinionCount_Implementation(int32 Amount)
{
	MinionCount = MinionCount + Amount;
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& GameplayTag) const
{
	if (GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	else if (GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	else if (GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	else if (GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

AActor* AAuraCharacterBase::GetAvatarActor_Implementation()
{
	return this;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

TArray<FTagsToMontage> AAuraCharacterBase::GetTagsToMontage_Implementation() const
{
	return TagsToMontages;
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

void AAuraCharacterBase::InitializaDefaultAttriutes() const
{
	InitalizePrimaryAttributes();
	InitalizeSecondaryAttributes();
	InitalizeVitalAttributes();
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AuraASC->AddCharacterAbilities(StartupAbilities);
}



