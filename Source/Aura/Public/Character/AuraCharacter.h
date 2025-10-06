// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_MULTICAST_DELEGATE(FOnControlledStateChanged);
#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public  IAuraInterface
{
	GENERATED_BODY()
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	
public:
	AAuraCharacter();
	virtual void InitAbilityActorInfo() override;
	/**Combat Interface**/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	/**Combat Interface**/

	/**Aura Interface**/
	virtual void AddToXP_Implementation(int32 InXP) override;;
	virtual void AddToAttributePoints_Implementation(int32 InPoints) override;
	virtual void AddToPlayerLevel_Implementation(int32 InLevel) const override;
	virtual void AddToSpellPoints_Implementation(int32 InPoints) override;
	virtual void LevelUp_Implementation(int32 CurLevel, int32 NewLevel) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 CurLevel, int32 NewLevel) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 CurLevel, int32 NewLevel) const override;
	virtual int32 FindLevelForExperience_Implementation(int32 Experience) const override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* Material) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& PlayerStartTag) override;
	/**Aura Interface**/

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual  void OnStunTagCountChanged(const FGameplayTag GameplayTag, int32 Count) override;
	
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_IsControlled)
	bool bIsControlled = false;
	bool GetIsControlled() const {return bIsControlled; }

	UFUNCTION()
	void OnRep_IsControlled();
	FOnControlledStateChanged OnControlledStateChanged;
	
	void UpdateIsControlled();
protected:
	
	UPROPERTY()
	FTimerHandle DeathTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.0f;
	
	virtual void InitializaDefaultAttriutes() const override;

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent *TopDownCameraComponent;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* LevelUpNiagaraComponent;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UFUNCTION()
	void LoadProgress();

	void OnLevelChanged();
};
