// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <Player/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>

#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


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

void AAuraCharacter::OnLevelChanged()
{
	// 重置控制器旋转
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        
		// 对于俯视角游戏，可能需要禁用鼠标看向
		PC->bShowMouseCursor = true;
		PC->SetIgnoreLookInput(true);
	}
    
	// 重置角色旋转
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
    
	// 确保移动组件也重置方向
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->StopMovementImmediately();
		MovementComp->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // 适合俯视角的旋转速率
	}
    
	UE_LOG(LogTemp, Log, TEXT("Character orientation reset after level change"));
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAuraCharacter::OnLevelChanged, 0.1f, false);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	//给服务器初始化用的
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	
	LoadProgress();
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (AuraGameModeBase)
	{
		AuraGameModeBase->LoadWorldState(GetWorld());
	}
	OnControlledStateChanged.AddUObject(this, &AAuraCharacter::UpdateIsControlled);
}

void AAuraCharacter::LoadProgress()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (AuraGameModeBase)
	{
		ULoadScreenSaveGame* LoadData =  AuraGameModeBase->GetCurGameInstanceSaveData();
		if (LoadData == nullptr) return;
		if (LoadData->IsFirstSaveData)
		{
			AddCharacterAbilities();//添加开始能力
		}
		else
		{
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetPlayerLevel(LoadData->Level);
				AuraPlayerState->SetXP(LoadData->XP);
				AuraPlayerState->SetAttributePoints(LoadData->AttributePoints);
				AuraPlayerState->SetSpellPoints(LoadData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitializeDefaultAttributes_SetByCaller(this, AbilitySystemComponent, LoadData);

			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->LoadCharacterAbilities(LoadData);
			}
		}
	}


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
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathDelegate;
	DeathDelegate.BindLambda([this]()
	{
		AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGameModeBase)
		{
			AuraGameModeBase->PlayerDie(this);
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DeathDelegate, DeathTime, false);
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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
	AuraASC->ServerUpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
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

	IAuraPlayerInterface::Execute_AddToAttributePoints(this,AttributePointsReward);
	IAuraPlayerInterface::Execute_AddToSpellPoints(this,SpellPointsReward);
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

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* Material)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))
	{
		PC->ShowMagicCircle(Material);
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(GetController()))
	{
		PC->HideMagicCircle();
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& PlayerStartTag)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		if (!HasAuthority()) return;
		
		ULoadScreenSaveGame* Data = AuraGameModeBase->GetCurGameInstanceSaveData();
		if (Data == nullptr) return;
		Data->PlayerStartTag = PlayerStartTag;
		Data->IsFirstSaveData = false;
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			Data->Level = AuraPlayerState->GetPlayerLevel();
			Data->XP = AuraPlayerState->GetXP();
			Data->AttributePoints = AuraPlayerState->GetAttributePoints();
			Data->SpellPoints = AuraPlayerState->GetSpellPoints();
		}

		if (GetAttributeSet())
		{
			Data->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
			Data->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
			Data->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
			Data->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		}

		FForEachAbility ForEachAbility;
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		Data->SaveAbilities.Empty();
		ForEachAbility.BindLambda([this, AuraASC, Data](const FGameplayAbilitySpec& AbilitySpec)
		{
			FGameplayTag AbilityTag = AuraASC->GetAbilityTagForAbilitySpec(AbilitySpec);
			UAbilityInfo* AbilityInfos = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
			FAuraAbilityInfo AbilityInfo = AbilityInfos->FindAbilityInfoForAbilityTag(AbilityTag);
			
			FSaveAbilityInfo SaveAbilityInfo;
			SaveAbilityInfo.AbilityTag = AbilityTag;
			SaveAbilityInfo.AbilitySlot = AuraASC->GetInputTagForAbilitySpec(AbilitySpec);
			SaveAbilityInfo.AbilityStatus = AuraASC->GetStatusTagForAbilitySpec(AbilitySpec);
			SaveAbilityInfo.AbilityLevel = AbilitySpec.Level;
			
			SaveAbilityInfo.Ability = AbilityInfo.AbilityClass;
			SaveAbilityInfo.AbilityType = AbilityInfo.AbilityType;
			Data->SaveAbilities.AddUnique(SaveAbilityInfo);
		});
		AuraASC->ForEachAbility(ForEachAbility);
		
		AuraGameModeBase->SavingCurGameInstanceProgress(Data);
	}
}

void AAuraCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraCharacter, bIsControlled);
	
}

void AAuraCharacter::OnStunTagCountChanged(const FGameplayTag GameplayTag, int32 Count)
{
	bIsStun = Count > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStun ? 0.f : BaseWalkSpeed;
	OnControlledStateChanged.Broadcast();

}

/*  */
void AAuraCharacter::InitAbilityActorInfo()
{
	TObjectPtr<AAuraPlayerState> AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
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
	OnAscRegistered.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::OnStunTagCountChanged);
}

void AAuraCharacter::OnRep_IsControlled()
{
}

void AAuraCharacter::UpdateIsControlled()//是否控制不能按键， 走路等
{
	bIsControlled = bIsStun;
}

void AAuraCharacter::InitializaDefaultAttriutes() const
{
	Super::InitializaDefaultAttriutes();
}







