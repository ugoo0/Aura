// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraFireBolt.h"

#include "AuraAbilitySystemLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	
	
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc;
	if (Level == 1)
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>FireBolt</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
			//Desc
			"<Default>Launches a fire bolt, exploding on impact and dealing :</>"
			"<Damage>%d</>" //Damage
			"<Default>fire Damage with a chance to burn</>\n"
			
			),
			Level,
			ManaCost,
			Cooldown,
			Damage);
	}
	else
	{
		Desc = FString::Printf(TEXT(
			//Title
			"<Title>FireBolt</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost : </><ManaCost>%d</>\n"
			//Cooldown
			"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
			//Desc
			"<Default>Launches %d fire bolts, exploding on impact and dealing :</>"
			"<Damage>%d</>" //Damage
			"<Default>fire Damage with a chance to burn</>\n"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(NumProjectiles, Level),
			Damage);
		}	
	return Desc;
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByLevel(Level);
	const int32 ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCoolDown(Level);
	FString Desc = FString::Printf(TEXT(
		//Title
		"<Title>FireBolt</>\n\n "
		//Level
		"<Small>Level : </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost : </><ManaCost>%d</>\n"
		//Cooldown
		"<Small>Cooldown : </><ManaCost>%.1f</>\n\n"
		//Desc
		"<Default>Launches %d fire bolts, exploding on impact and dealing :</>"
		"<Damage>%d</>" //Damage
		"<Default>fire Damage with a chance to burn</>\n"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(NumProjectiles, Level),
		Damage);
	return Desc;
}

void UAuraFireBolt::SpawnProjectiles(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch,
	float OverridePitch, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation =  (TargetLocation- SocketLocation).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = OverridePitch;
		}
		FVector Forward = Rotation.Vector();
		SpreadNums = FMath::Min(MaxSpreadNums, GetAbilityLevel());
		TArray<FRotator> Directions = UAuraAbilitySystemLibrary::EvenlyRotatedRotators(Forward, FVector::UpVector, Spread, SpreadNums);

		for (FRotator Rot : Directions)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetRotation(Rot.Quaternion());
			SpawnTransform.SetLocation(SocketLocation);
			AAuraProjectile* Projectile =  GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FDamageEffectParams Params = GetDamageEffectParamsForTarget();
			Projectile->DamageEffectParams = Params;

			if (IsValid(HomingTarget) && HomingTarget->Implements<UCombatInterface>())
			{
				Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
			}
			else
			{
				HomingSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->ProjectileMovement->HomingTargetComponent = HomingSceneComponent;
				HomingSceneComponent->SetWorldLocation(TargetLocation);
			}
			Projectile->ProjectileMovement->bIsHomingProjectile = bIsHoming;
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			
			Projectile->FinishSpawning(SpawnTransform);

			
		}

	}
}




