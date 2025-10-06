// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraEnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Interaction/AuraSaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public IAuraSaveInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();

	/* IAuraSaveInterface */
	virtual void LoadData_Implementation() override;
	/* IAuraSaveInterface */
	
	UPROPERTY(SaveGame, BlueprintReadOnly)
	bool bReached = false;

	UPROPERTY(EditAnywhere, Category="SpawnPoints")
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;
	
protected:

	UFUNCTION()
	virtual void OnBoxOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
