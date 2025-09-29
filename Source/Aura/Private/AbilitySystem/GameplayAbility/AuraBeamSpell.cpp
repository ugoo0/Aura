// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraBeamSpell.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitActor = HitResult.GetActor();
		MouseHitLocation = HitResult.ImpactPoint;
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter)
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			FHitResult HitResult;
			ActorsToIgnore.Add(OwnerCharacter);
			FVector StartLocation = Weapon->GetSocketLocation("TipSocket");
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
				StartLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitActor = HitResult.GetActor();
				MouseHitLocation = HitResult.ImpactPoint;

				ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor);
				if (CombatInterface && !CombatInterface->GetActorDeadDelegate().IsAlreadyBound(this, &UAuraBeamSpell::MouseHitActorDead))
				{
					CombatInterface->GetActorDeadDelegate().AddDynamic(this, &UAuraBeamSpell::MouseHitActorDead);
					if (CombatInterface->GetActorDeadDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalActorDead))
					{
						CombatInterface->GetActorDeadDelegate().RemoveDynamic(this, &UAuraBeamSpell::AdditionalActorDead);
					}
				}
			}
		}
		
	}
	
}

void UAuraBeamSpell::StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> OutOverlayActors;
	OutAdditionalTargets.Reset();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	ActorsToIgnore.Add(MouseHitActor);
	int32 SpreadNum = FMath::Min(GetAbilityLevel() - 1, MaxBeamSpreadNum);
	UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(GetAvatarActorFromActorInfo(), OutOverlayActors, ActorsToIgnore, MouseHitActor->GetActorLocation(), BeamRadius);
	OutOverlayActors.RemoveAll([](AActor* Actor) { return Actor == nullptr; });
	if (OutOverlayActors.Num() < SpreadNum)
	{
		OutAdditionalTargets = OutOverlayActors;
	}
	else
	{
		using ElementType = decltype(OutOverlayActors)::ElementType;
		Algo::Sort(OutOverlayActors,
			[this](ElementType A, ElementType B)
			{
				if (!MouseHitActor) return false;
				float DistanceA = FVector::DistSquared(A->GetActorLocation(), MouseHitActor->GetActorLocation());
				float DistanceB = FVector::DistSquared(B->GetActorLocation(), MouseHitActor->GetActorLocation());
				return  DistanceA < DistanceB;
			});
		for (int32 i = 0; i < SpreadNum; i++)
		{
			OutAdditionalTargets.AddUnique(OutOverlayActors[i]);
			//UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), OutOverlayActors[i]->GetActorLocation(),20,12,FLinearColor::Red,10);
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(OutOverlayActors[i]);
			if (CombatInterface && !CombatInterface->GetActorDeadDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalActorDead))
			{
				CombatInterface->GetActorDeadDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalActorDead);
				if (CombatInterface->GetActorDeadDelegate().IsAlreadyBound(this, &UAuraBeamSpell::MouseHitActorDead))
				{
					CombatInterface->GetActorDeadDelegate().RemoveDynamic(this, &UAuraBeamSpell::MouseHitActorDead);
				}
			}
		}
	}
}
