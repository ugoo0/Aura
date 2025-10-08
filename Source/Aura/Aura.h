// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//高亮类 后处理
#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

//自定义碰撞通道，用于处理一些特殊对象的碰撞检测
#define ECC_PROJECTILE ECollisionChannel::ECC_GameTraceChannel1
#define ECC_TARGET ECollisionChannel::ECC_GameTraceChannel2
#define ECC_EXCLUDEPLAYERS ECollisionChannel::ECC_GameTraceChannel3
