#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;


UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	FOnDamageTakeSignature OnDamageTakeSignature;
	
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet()  const {return AttributeSet;};
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveMeshTimeline(UMaterialInstanceDynamic* DynamicMaterial);

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveWeaponTimeline(UMaterialInstanceDynamic* DynamicMaterial);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<UMaterialInstance> MeshDissolvedMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	TObjectPtr<UMaterialInstance> WeaponDissolvedMaterialInstance;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	/*combat interface*/
	virtual UAnimMontage* GetMelleAttackMontage_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& GameplayTag) const override;

	virtual  AActor* GetAvatarActor_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual TArray<FTagsToMontage> GetTagsToMontage_Implementation() const override;
	virtual UNiagaraSystem* GetHitNiagaraSystem_Implementation() const override;
	virtual FTagsToMontage GetTagsToMontageByMontageTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMaxMinionCount_Implementation() override;
	virtual void AddMinionCount_Implementation(int32 Amount) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual ECharacterClassType GetCharacterClassType_Implementation() const override;

	FOnASCRegistered OnAscRegistered;
	FOnActorDead OnActorDead;

	virtual FOnActorDead& GetActorDeadDelegate() override;
	virtual FOnASCRegistered& GetASCRegisteredDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	virtual FOnDamageTakeSignature& GetOnDamageTakeDelegate() override;	
	/*end combat interface*/

	/*NiagaraComponent*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent>  DebuffBurnNiagaraComponent1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDebuffNiagaraComponent>  DebuffStunNiagaraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent>  PassiveAttachComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPassiveNiagaraComponent>  HaloOfProtectionNiagaraComponent1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPassiveNiagaraComponent>  LifeSiphonNiagaraComponent1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPassiveNiagaraComponent>  ManaSiphonNiagaraComponent1;

	void SetCharacterClass(ECharacterClassType InCharacterClassType) {CharacterClassType = InCharacterClassType;};
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
	
    UPROPERTY(EditAnywhere, Category="Combat")
    FName TailSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* MelleAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<FTagsToMontage>  TagsToMontages;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	int32 MaxMinionCount;
	int32 MinionCount = 0;

	UPROPERTY(BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charcter Class Default")
	ECharacterClassType CharacterClassType = ECharacterClassType::Warrior;

	/*Debuff States*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_BurnStateChanged)
	bool bIsBurn = false;

	UFUNCTION()
	void OnRep_BurnStateChanged();

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_StunStateChanged)
	bool bIsStun = false;

	UFUNCTION()
	virtual void OnRep_StunStateChanged();

	UFUNCTION()
	virtual  void OnStunTagCountChanged(const FGameplayTag GameplayTag, int32 Count);

	UPROPERTY(BlueprintReadOnly,Replicated)
	bool bIsBeingShock = false;

	virtual void SetIsBeingShock_Implementation(bool InIsBeingShock) override;
	virtual bool GetIsBeingShock_Implementation() const override;
	/*Debuff States*/


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 600.f;
	
private:
	UPROPERTY(EditAnywhere, Category = "ASC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "ASC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
};

