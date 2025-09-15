// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* OwingApawn = AIOwner->GetPawn();
	const FName TargetTag = OwingApawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwingApawn->GetWorld(), TargetTag, ActorWithTag);

	AActor* CloseActor = nullptr;
	float CloseDistance = TNumericLimits<float>::Max();
	for (const auto& actor : ActorWithTag)
	{
		if (IsValid(actor) && IsValid(OwingApawn))
		{
			float Distance = OwingApawn->GetDistanceTo(actor);
			if (Distance < CloseDistance)
			{
				CloseActor = actor;
				CloseDistance = Distance;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowKeySelector, CloseActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTarget, CloseDistance);
}
