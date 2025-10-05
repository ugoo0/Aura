// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/AuraSaveInterface.h"
#include "Interaction/HighLightInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckPoint : public APlayerStart, public IAuraSaveInterface, public IHighLightInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,SaveGame)
	bool bReached = false;


	UPROPERTY(EditDefaultsOnly)
	bool bOverrideBindSphereOverlay = true;
	
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* InstanceDynamic);

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffect();

	/* AuraSave Interface */
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadData_Implementation() override;
	/* AuraSave Interface */

	/** IHighLightInterface **/
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& Destination) override;
	/** IHighLightInterface **/
protected:
	UFUNCTION()
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> MoveToComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
private:
	

};
