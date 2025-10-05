// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	/* AuraSave Interface */
	virtual void LoadData_Implementation() override;
	/* AuraSave Interface */

	/** IHighLightInterface **/
	virtual void HighlightActor_Implementation() override;
	/** IHighLightInterface **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapEnrance")
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MapEnrance")
	FName DestinationPlayerStartTag;

protected:
	virtual void OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void LoadDestinationLevel();
	UFUNCTION()
	void OnDestinationLevelLoaded();
	UFUNCTION()
	void OnDestinationLevelShown();
	void TeleportPlayerToDestination();
	APlayerStart* FindPlayerStartByTag();
};
