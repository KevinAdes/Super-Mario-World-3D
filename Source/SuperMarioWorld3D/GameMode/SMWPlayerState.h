// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SMWPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMARIOWORLD3D_API ASMWPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	FName PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 Lives;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Coins;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Points;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor Skin;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor Shirt;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor Pants;
};
