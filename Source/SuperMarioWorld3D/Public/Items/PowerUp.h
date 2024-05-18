// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible.h"
#include "PowerUp.generated.h"

class UPowerUp;

USTRUCT(BlueprintType)
struct FPowerUpData : public FTableRowBase
{
	GENERATED_BODY()
	
	TObjectPtr<UPowerUp> PowerUp;
	int32 PowerValue = 0;
};

/**
 * 
 */
UCLASS()
class SUPERMARIOWORLD3D_API UPowerUp : public UObject, public ICollectible
{
	GENERATED_BODY()
public:
	void ItemBoxFall();
	
};
