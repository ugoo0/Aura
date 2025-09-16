// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet()  const {return AttributeSet;};
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveMeshTimeline(UMaterialInstanceDynamic* DynamicMaterial);

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveWeaponTimeline(UMaterialInstanceDynamic* DynamicMaterial);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<UMaterialInstance> MeshDissolvedMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<UMaterialInstance> WeaponDissolvedMaterialInstance;

	/*combat interface*/
	virtual UAnimMontage* GetMelleAttackMontage_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& GameplayTag) const override;

	virtual  AActor* GetAvatarActor_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual TArray<FTagsToMontage> GetTagsToMontage_Implementation() const override;
	virtual int32 GetPlayerLevel() const override;
	/*end combat interface*/
	
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* MelleAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<FTagsToMontage>  TagsToMontages;
	
	bool bDead = false;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesGameEffectClass;//初始化PrimaryAttributes的gameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesGameEffectClass;//初始化SecondaryAttributes的gameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesGameEffectClass;//初始化VitalAttributes的gameplayEffect类
	
	void ApplyGameEffectByGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;
	void InitalizePrimaryAttributes() const;
	void InitalizeSecondaryAttributes() const;
	void InitalizeVitalAttributes() const;

	virtual void InitializaDefaultAttriutes() const;
	
	void AddCharacterAbilities() const;
private:

	UPROPERTY(EditAnywhere, Category = "ASC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

inline int32 AAuraCharacterBase::GetPlayerLevel() const
{
	return 0;
}
