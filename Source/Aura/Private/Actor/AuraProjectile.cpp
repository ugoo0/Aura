// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}



void AAuraProjectile::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectParams.SourceASC) return;
	if (DamageEffectParams.SourceASC->GetAvatarActor() == OtherActor) return;

	if (UAuraAbilitySystemLibrary::IsFriend(OtherActor,DamageEffectParams.SourceASC->GetAvatarActor())) return;
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactNiagara,GetActorLocation(),FRotator::ZeroRotator);
	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
		LoopingSoundComp->DestroyComponent();
	}

	if (HasAuthority())
	{
		if (!bOverlay)
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.TargetASC = TargetASC;
				DamageEffectParams.DeathImpulse = DeathImpulse;


				FRotator ActorRotate = GetActorRotation();
				ActorRotate.Pitch = 45.f;
				DamageEffectParams.KnockbackForce = ActorRotate.Vector() * DamageEffectParams.KnockbackForceMagnitude;
				
				UAuraAbilitySystemLibrary::ApplyDamageEffectForDamageEffectParms(DamageEffectParams.TargetASC->GetAvatarActor() ,DamageEffectParams);
			}
		
			Destroy();
		}
	}
	else
	{
		bOverlay = true;
	}
}

void AAuraProjectile::HomingTargetDead(AActor* Actor)
{
	ProjectileMovement->bIsHomingProjectile = false;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlay);
	LoopingSoundComp = UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
	SetLifeSpan(LifeSpan);
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
		LoopingSoundComp->DestroyComponent();
	}
	if (!bOverlay && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactNiagara,GetActorLocation(),FRotator::ZeroRotator);
		if (LoopingSoundComp) LoopingSoundComp->Stop();
	}
	Super::Destroyed();
}


