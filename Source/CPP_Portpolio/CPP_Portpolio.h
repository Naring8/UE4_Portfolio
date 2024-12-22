// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CreateDefaultSubobjectArray(Array, Index)\
(Array[Index] = CreateDefaultSubobject<std::remove_reference<decltype(*Array[Index])>::type>(FName(#Array"[" + FString::FromInt(Index) + "]")))

/* Print to Screen Code
if (GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Jump Velocity : %f"), GetCharacterMovement()->JumpZVelocity));
*/

#define ClimbCollision ECollisionChannel::ECC_GameTraceChannel1
