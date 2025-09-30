// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraFireBlast.h"

#include "AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc= FString::Printf(TEXT(
	//Title
	"<Title>FireBlast</>\n\n"

	//Level
	"<Small>Level : </><Level>%d</>\n"
	//ManaCost
	"<Small>ManaCost : </><ManaCost>%d</>\n"
	//Cooldown
	"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
	//Desc
	"<Default>Launches %d "
	"fire bolts in all directions, each coming back and "
	"exploding upon return, causing </>"
	"<Damage>%d</>" //Damage
	"<Default> radial fire Damage with a chance to burn.</>\n"
	),
	Level,
	ManaCost,
	Cooldown,
	NumOfFirebolt,
	Damage);
	return  Desc;
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc= FString::Printf(TEXT(
	//Title
	"<Title>Next Level</>\n\n"

	//Level
	"<Small>Level : </><Level>%d</>\n"
	//ManaCost
	"<Small>ManaCost : </><ManaCost>%d</>\n"
	//Cooldown
	"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"
	//Desc
	"<Default>Launches %d "
	"fire bolts in all directions, each coming back and "
	"exploding upon return, causing </>"
	"<Damage>%d</>" //Damage
	"<Default> radial fire Damage with a chance to burn.</>\n"
	),
	Level,
	ManaCost,
	Cooldown,
	NumOfFirebolt,
	Damage);
	return  Desc;
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlyRotatedRotators(Forward, FVector::UpVector,360.f-360.0f/12.0f, NumOfFirebolt);
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTrans;
		SpawnTrans.SetLocation(Location);
		SpawnTrans.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTrans,
			GetAvatarActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->OnFinalExplodeDelegate.AddDynamic(this, &UAuraFireBlast::OnFinalExplode);
		FireBall->DamageEffectParams = GetDamageEffectParamsForTarget();
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTrans);
	}
	return FireBalls;
}
