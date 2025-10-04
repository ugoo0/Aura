// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AuraSaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart, public IAuraSaveInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	bool bReached = false;
	
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* InstanceDynamic);

	void HandleGlowEffect();

	/* AuraSave Interface */
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadData_Implementation() override;
	/* AuraSave Interface */
protected:
	UFUNCTION()
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
};
