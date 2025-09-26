// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDead, AActor*, Actor);

enum class ECharacterClassType : uint8;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTagsToMontage
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageGameplayTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag SocketGameplayTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& GameplayTag) const;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void FacingToTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetMelleAttackMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget();
	virtual void Die(const FVector& DeathImpulse) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    AActor* GetAvatarActor();
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsDead() const;	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTagsToMontage> GetTagsToMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetHitNiagaraSystem() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTagsToMontage GetTagsToMontageByMontageTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMaxMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClassType GetCharacterClassType() const;

	virtual  FOnASCRegistered& GetASCRegisteredDelegate() = 0;

	virtual FOnActorDead& GetActorDeadDelegate() = 0;
};
