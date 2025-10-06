// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Actor/AuraEffectActor.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AAuraEnemy::AAuraEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarWidget->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	BaseWalkSpeed = 200.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraEnemy::HighlightActor_Implementation()
{
	//bHighlighted = true; //测试数据
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	//bHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

void AAuraEnemy::SetMoveToLocation_Implementation(FVector& Destination)
{
	
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClassType != ECharacterClassType::Warrior);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent, CharacterClassType);
	}
	
	if (UAuraUserWidget* UserWidget = Cast<UAuraUserWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AAuraEnemy::HitReactTagChanged);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AblityActorInfoSet();
	if (HasAuthority())
	{
		InitializaDefaultAttriutes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::OnStunTagCountChanged);
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(TimeRemoveAfterDie);
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	SpawnLoot();
	Super::Die(DeathImpulse);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* Target)
{
	CombatTarget = Target;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void AAuraEnemy::OnStunTagCountChanged(const FGameplayTag GameplayTag, int32 Count)
{
	bIsStun = Count > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStun ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bIsStun"), bIsStun);
	}
}

void AAuraEnemy::InitializaDefaultAttriutes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClassType,Level, AbilitySystemComponent);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag Tag, int32 NewTagCount)
{
	bHitReacting = NewTagCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAuraEnemy::Destroyed()
{
	Super::Destroyed();
}

void AAuraEnemy::SpawnLoot()
{
	ULootTiers* LootTiers = UAuraAbilitySystemLibrary::GetLootTiers(this);
	if (!LootTiers) return;
	TArray<FLootItem> LootItems = LootTiers->GetLootItems();
	if (LootItems.Num() < 1) return;
	const FVector Forward = GetActorForwardVector();
	const FVector Location = GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlyRotatedRotators(Forward, FVector::UpVector, 360.f-360.f/LootItems.Num(), LootItems.Num());
	int32 Index = 0;

	FTimerDelegate SpawnLootDelegate;
	//Lambda值引用，使用mutable这样可以修改Index
	SpawnLootDelegate.BindLambda([this,Index, LootItems,Rotators,Location]() mutable 
	{
		if (Index > LootItems.Num() - 1)
		{
			if (SpawnLootTimer.IsValid())
			{
				GetWorldTimerManager().ClearTimer(SpawnLootTimer);
			}
			return;
		}
		FRotator Rotator = Rotators[Index];
		FActorSpawnParameters SpawnParameters;
		FTransform Transform;
		FVector SpawnLocation = Location + Rotator.Vector()*FMath::FRandRange(MinDistanceToSpawn, MaxDistanceToSpawn);
		Transform.SetRotation(Rotator.Quaternion());
		Transform.SetLocation(SpawnLocation);
		FLootItem SpawnItem = LootItems[Index];
		Index = Index + 1;
		AAuraEffectActor* AuraEffectActor = GetWorld()->SpawnActorDeferred<AAuraEffectActor>(SpawnItem.ItemClass, Transform);
		if (SpawnItem.bLootLevelOverride)
		{
			AuraEffectActor->SetLevel(Level);
		}
		AuraEffectActor->FinishSpawning(Transform);
	});
	GetWorldTimerManager().SetTimer(SpawnLootTimer, SpawnLootDelegate, 0.1f, true);
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}
