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

public:

	virtual void BeginPlay() override;
	
	int32 GetPowerState() const { return PowerState; }

	int32 GetStoredPowerUpValue() const { return StoredPowerUpValue; }
	
protected:
	UPROPERTY(BlueprintReadWrite)
	FName PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 Lives;
	
	UPROPERTY(BlueprintReadWrite)
	int32 PowerState;

	UPROPERTY(BlueprintReadWrite)
	int32 StoredPowerUpValue;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Coins;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Points;

	UPROPERTY(BlueprintReadWrite)
	int32 StarPoints;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor Skin;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor Shirt;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor Pants;


};
