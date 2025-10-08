// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint/MapEntrance.h"

#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "Interaction/AuraPlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadData_Implementation()
{
	//Do nothing
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
	CheckPointMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
}

void AMapEntrance::OnSphereOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UAuraPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGameModeBase->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IAuraPlayerInterface::Execute_SaveProgress(OtherActor,DestinationPlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationMap);
	    // LoadDestinationLevel();
	}
}

void AMapEntrance::LoadDestinationLevel()
{
    UWorld* World = GetWorld();
    if (!World || DestinationMap.IsNull()) return;
    
    FString LevelName = DestinationMap.GetAssetName();
    
    // 动态加载目标关卡
    bool bOutSuccess;
    ULevelStreamingDynamic* LoadedLevel = ULevelStreamingDynamic::LoadLevelInstance(
        World, 
        LevelName, 
        GetActorLocation(),  // 或者使用特定位置
        FRotator::ZeroRotator, 
        bOutSuccess
    );
    
    if (LoadedLevel)
    {
        // 设置关卡加载完成后的回调
        LoadedLevel->OnLevelLoaded.AddDynamic(this, &AMapEntrance::OnDestinationLevelLoaded);
        LoadedLevel->OnLevelShown.AddDynamic(this, &AMapEntrance::OnDestinationLevelShown);
        
        // 可选：隐藏当前关卡
        // UnloadCurrentLevel();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load level: %s"), *LevelName);
    }
}

void AMapEntrance::OnDestinationLevelLoaded()
{
    // 关卡加载完成后的处理
    UE_LOG(LogTemp, Log, TEXT("Destination level loaded successfully"));
    
    // 在这里处理玩家传送等逻辑
    TeleportPlayerToDestination();
}

void AMapEntrance::OnDestinationLevelShown()
{
    // 关卡显示完成后的处理
    UE_LOG(LogTemp, Log, TEXT("Destination level shown"));
}

void AMapEntrance::TeleportPlayerToDestination()
{
    // 查找目标玩家起始点
    APlayerStart* DestinationStart = FindPlayerStartByTag();
    if (!DestinationStart) return;
    
    // 获取玩家角色
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (!PlayerCharacter) return;
    
    // 传送玩家到目标位置
    PlayerCharacter->SetActorLocation(DestinationStart->GetActorLocation());
    PlayerCharacter->SetActorRotation(DestinationStart->GetActorRotation());
    
    // 重置控制器旋转以确保移动方向正确
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetControlRotation(DestinationStart->GetActorRotation());
    }
    
    UE_LOG(LogTemp, Log, TEXT("Player teleported to destination"));
}

APlayerStart* AMapEntrance::FindPlayerStartByTag()
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;
    
    // 查找所有PlayerStart
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);
    
    for (AActor* Actor : PlayerStarts)
    {
        APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
        if (PlayerStart && PlayerStart->PlayerStartTag == DestinationPlayerStartTag)
        {
            return PlayerStart;
        }
    }
    
    // 如果没有找到特定tag的PlayerStart，返回第一个
    if (PlayerStarts.Num() > 0)
    {
        return Cast<APlayerStart>(PlayerStarts[0]);
    }
    
    return nullptr;
}