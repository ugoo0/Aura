// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"
TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	FVector LeftVector = Forward.RotateAngleAxis(SpawnSpread/2.f, FVector::UpVector);
	FVector RightVector = Forward.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);

	const float RotateOffset = SpawnSpread/NumMinions;

	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftVector*MaxSpawnDistance, 4.f, FLinearColor::Red, 3.f);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightVector*MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);

	TArray<FVector> SpawnLocations;
	float AcceptableHeight = 400.f;
	for (int i = 0; i < NumMinions; ++i)
	{
		FVector MinionForward = RightVector.RotateAngleAxis(RotateOffset*i, FVector::UpVector);
		FVector MinionLocation = Location + MinionForward * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		// UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), MinionLocation, 70.f, 12, FLinearColor::Blue, 3.f);
		// UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), Location + MinionForward*MinSpawnDistance, 10.f, 12, FLinearColor::Red, 3.f);
		// UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), Location + MinionForward*MaxSpawnDistance, 10.f, 12, FLinearColor::Red, 3.f);
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, MinionLocation + FVector::UpVector*AcceptableHeight, MinionLocation - FVector::UpVector*AcceptableHeight, ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			MinionLocation = HitResult.ImpactPoint;
			SpawnLocations.Add(MinionLocation);
		}
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetSpawnClassRandom()
{
	int SelectIndex = FMath::RandRange(0, MinionClass.Num()-1);
	return MinionClass[SelectIndex];
}
