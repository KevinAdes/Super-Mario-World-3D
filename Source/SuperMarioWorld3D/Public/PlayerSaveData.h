// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class ESaveDataValue : uint8
{
	SD_lives			UMETA(DisplayName = "Lives"),
	SD_coins			UMETA(DisplayName = "Coins"),
	SD_points			UMETA(DisplayName = "Points"),
	SD_starPoints		UMETA(DisplayName = "Star Points"),
	SD_powerState		UMETA(DisplayName = "Power State"),
};

USTRUCT(BlueprintType)
struct FSaveData
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
	int32 powerState = 1;

	FSaveData(){}

	void SetData(const ESaveDataValue type, int32 value)
	{
		switch (type)
		{
		case ESaveDataValue::SD_lives:
			lives = value;
		case ESaveDataValue::SD_coins:
			coins = value;
		case ESaveDataValue::SD_points:
			points = value;
		case ESaveDataValue::SD_starPoints:
			starPoints = value;
		case ESaveDataValue::SD_powerState:
			powerState = value;
		default:;
		}
	}

	int GetData(const ESaveDataValue type)
	{
		switch (type)
		{
		case ESaveDataValue::SD_lives:
			return lives;
		case ESaveDataValue::SD_coins:
			return coins;
		case ESaveDataValue::SD_points:
			return points;
		case ESaveDataValue::SD_starPoints:
			return starPoints;
		case ESaveDataValue::SD_powerState:
			return powerState;
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
	FSaveData SaveData;

	UFUNCTION(BlueprintCallable)
	void SetSaveData(ESaveDataValue type, int32 value);

	UFUNCTION(BlueprintCallable)
	int32 GetSaveData(ESaveDataValue type);
	
	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static UPlayerSaveData* LoadPlayerSaveData();
	
	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static void SavePlayerSaveData(UPlayerSaveData* In_SaveGame);
};
