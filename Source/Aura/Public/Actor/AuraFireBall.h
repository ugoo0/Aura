// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFinalExplodeSignature, const AActor*, Target, const FDamageEffectParams&, Params);

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartTimeLine();
	
	AAuraFireBall();
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	FOnFinalExplodeSignature OnFinalExplodeDelegate;

	UFUNCTION(BlueprintCallable)
	void BroadCastFinalExplodeDelegateToTargets(TArray<AActor*> Targets, bool IsRadialDamage, FVector RadialOrigin = FVector::ZeroVector);

	virtual void OnHit() override;	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ApexDistance = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplodeDistance = 150.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)

	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RadialDamageOuterRadius = 70.f;
};
