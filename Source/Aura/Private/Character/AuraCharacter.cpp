// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <Player/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>

#include "AbilitySystem/Data/LevelInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	

	
	GetCharacterMovement() ->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClassType = ECharacterClassType::Elementalist;
}


void AAuraCharacter::PossessedBy(AController* NewController)
{
	//给服务器初始化用的
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities();//添加开始能力
}

void AAuraCharacter::OnRep_PlayerState()
{
	//给客户端初始化用的
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	TObjectPtr<AAuraPlayerState> AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	if (AuraPlayerState->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetPlayerLevel(AuraPlayerState);
	}
	return  1;
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}



void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InLevel);
	UAuraAbilitySystemComponent* AuraASC =  Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
	AuraASC->ServerUpdateAbilityStatus(ICombatInterface::Execute_GetPlayerLevel(AuraPlayerState));
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InPoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InPoints);
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		FRotator ToCameraRotation = (TopDownCameraComponent->GetComponentLocation()-LevelUpNiagaraComponent->GetComponentLocation()).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

void AAuraCharacter::LevelUp_Implementation(int32 CurLevel, int32 NewLevel)
{
	
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	int32 AttributePointsReward = GetAttributePointsReward_Implementation(CurLevel, NewLevel);
	int32 SpellPointsReward = GetSpellPointsReward_Implementation(CurLevel, NewLevel);

	IAuraInterface::Execute_AddToAttributePoints(this,AttributePointsReward);
	IAuraInterface::Execute_AddToSpellPoints(this,SpellPointsReward);
	AddToPlayerLevel_Implementation(NewLevel-CurLevel);


	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 CurLevel, int32 NewLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	int32 Points = 0;
	for (int32 Level = CurLevel + 1; Level  <= NewLevel; Level++)
	{
		FAuraLevelInfo AuraLevelInfo =  AuraPlayerState->AuraLevelInfo->GetLevelInfoForLevel(Level);
		Points = Points + AuraLevelInfo.SpellPointAward;
	}
	return Points;
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 CurLevel, int32 NewLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	int32 Points = 0;
	for (int32 Level = CurLevel + 1; Level  <= NewLevel; Level++)
	{
		FAuraLevelInfo AuraLevelInfo =  AuraPlayerState->AuraLevelInfo->GetLevelInfoForLevel(Level);
		Points = Points + AuraLevelInfo.AttributePointAward;
	}
	return Points;
}

int32 AAuraCharacter::FindLevelForExperience_Implementation(int32 Experience) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->AuraLevelInfo->FindLevelForExperience(Experience);
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	TObjectPtr<AAuraPlayerState> AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AblityActorInfoSet();//绑定gameplayEffect事件
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))//初始化OverlayWidget和OverlayWidgetController
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AuraHUD->InitOverlay(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializaDefaultAttriutes();
}

void AAuraCharacter::InitializaDefaultAttriutes() const
{
	Super::InitializaDefaultAttriutes();
}



