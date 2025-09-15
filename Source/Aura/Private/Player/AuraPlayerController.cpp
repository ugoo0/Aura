// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageWidgetComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputPressed,&ThisClass::AbilityInputReleased,&ThisClass::AbilityInputHeld);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftInputPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftInputReleased);
}
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}

}
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}
void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* Target,const FDamageState& DamageState) const
{
	if (IsValid(Target) && DamageTextComponentClass)
	{
		UDamageWidgetComponent* DamageComponent = NewObject<UDamageWidgetComponent>(Target, DamageTextComponentClass);
		DamageComponent->RegisterComponent();
		DamageComponent->AttachToComponent(Target->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageComponent->SetDamageText(Damage,DamageState);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = CurActor;
	CurActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	if (CurActor != LastActor)
	{
		if (LastActor)	LastActor->UnHighlightActor();
		if (CurActor)	CurActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurActor?true:false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (!GetASC()) return;
		GetASC()->AbilityInputHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftPressed)//释放技能
	{
		if (!GetASC()) return;
		GetASC()->AbilityInputHeld(InputTag);
		return;
	}
	else //移动
	{
		FollowTime = FollowTime + GetWorld()->GetDeltaSeconds();
		if (!CursorHit.bBlockingHit) return;
		CashedDestination = CursorHit.ImpactPoint;
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CashedDestination-ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection,1.f);
		}
		
	}
	
}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (!GetASC()) return;
		GetASC()->AbilityInputReleased(InputTag);
		return;
	}
	
	if (bTargeting || bShiftPressed)//释放技能
	{
		if (!GetASC()) return;
		GetASC()->AbilityInputReleased(InputTag);
		return;
	}
	else //移动
	{
		APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NaviPath =  UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CashedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& Point:NaviPath->PathPoints)
				{
					Spline->AddSplinePoint(Point,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),Point,10.f,10,FColor::Red,false,5.f);
				}
				if (NaviPath->PathPoints.Num() > 0)
				{
					CashedDestination = NaviPath->PathPoints[NaviPath->PathPoints.Num()-1];
					bAutoRunning = true;
				}

			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControllerPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World).GetSafeNormal();
		ControllerPawn->AddMovementInput(Direction,1.f);

		const float Distance = (CashedDestination-LocationOnSpline).Length();
		if (Distance < AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}
