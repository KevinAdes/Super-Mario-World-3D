// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerSaveDataValue : uint8
{
	SD_lives			UMETA(DisplayName = "Lives"),
	SD_coins			UMETA(DisplayName = "Coins"),
	SD_points			UMETA(DisplayName = "Points"),
	SD_starPoints		UMETA(DisplayName = "Star Points"),
	SD_powerState		UMETA(DisplayName = "Power State"),
	SD_powerValue		UMETA(DisplayName = "Held Item Power Value")
};

USTRUCT(BlueprintType)
struct FPlayerSaveDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 lives = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 coins = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 points = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 starPoints = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 powerState = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int32 HeldItemPowerValue = 0;
	
	FPlayerSaveDataStruct(){}

	void SetData(const EPlayerSaveDataValue type, int32 value)
	{
		switch (type)
		{
		case EPlayerSaveDataValue::SD_lives:
			lives = value;
		case EPlayerSaveDataValue::SD_coins:
			coins = value;
		case EPlayerSaveDataValue::SD_points:
			points = value;
		case EPlayerSaveDataValue::SD_starPoints:
			starPoints = value;
		case EPlayerSaveDataValue::SD_powerState:
			powerState = value;
		case EPlayerSaveDataValue::SD_powerValue:
			HeldItemPowerValue = value;
		default:;
		}
	}

	int GetData(const EPlayerSaveDataValue type)
	{
		switch (type)
		{
		case EPlayerSaveDataValue::SD_lives:
			return lives;
		case EPlayerSaveDataValue::SD_coins:
			return coins;
		case EPlayerSaveDataValue::SD_points:
			return points;
		case EPlayerSaveDataValue::SD_starPoints:
			return starPoints;
		case EPlayerSaveDataValue::SD_powerState:
			return powerState;
		case EPlayerSaveDataValue::SD_powerValue:
			return HeldItemPowerValue;
		default:
			return 0;
		}
	}
};

class UPlayerSaveData;
UCLASS()
class SUPERMARIOWORLD3D_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FPlayerSaveDataStruct SaveData;

	UFUNCTION(BlueprintCallable)
	void SetSaveData(EPlayerSaveDataValue type, int32 value);

	UFUNCTION(BlueprintCallable)
	int32 GetSaveData(EPlayerSaveDataValue type);
	
	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static UPlayerSaveData* LoadPlayerSaveData();
	
	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static void SavePlayerSaveData(UPlayerSaveData* In_SaveGame);
};
