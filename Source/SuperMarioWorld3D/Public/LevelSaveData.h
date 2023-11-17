// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSpawn.h"
#include "GameFramework/SaveGame.h"
#include "LevelSaveData.generated.h"

/**
 * 
 */

USTRUCT()
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 TimeLimit;
};

USTRUCT(BlueprintType)
struct FLevelSaveDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	TArray<APlayerSpawn*> SpawnLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	int SpawnIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	bool MidPoleCleared;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	bool DragonCoinsCollected;
	
	void SetSpawnLocations(TArray<APlayerSpawn*> Spawns)
	{
		SpawnLocations = Spawns;
	}

	TArray<APlayerSpawn*> GetSpawnLocations()
	{
		return SpawnLocations;
	}

	void SetSpawnIndex(int CurrentSpawn)
	{
		SpawnIndex = CurrentSpawn;
	}

	int GetSpawnIndex()
	{
		return SpawnIndex;
	}

	void SetMidPoleCleared(bool isCleared)
	{
		MidPoleCleared = isCleared;
	}

	bool GetMidPoleCleared()
	{
		return MidPoleCleared;
	}

	void SetDragonCoinsCollected(bool CoinsCollected)
	{
		DragonCoinsCollected = CoinsCollected;
	}

	bool GetDragonCoinsCollected()
	{
		return DragonCoinsCollected;
	}
};

class ULevelSaveData;
UCLASS()
class SUPERMARIOWORLD3D_API ULevelSaveData : public USaveGame
{
	GENERATED_BODY()
	
	ULevelSaveData();

public:
	
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FString, FLevelSaveDataStruct> SavedLevelMap;

	UFUNCTION(BlueprintCallable)
	void AddSlot(FString CurrentLevel);
	
	UFUNCTION(BlueprintCallable)
	void SetLevelSpawns(FString CurrentLevel, TArray<APlayerSpawn*> Spawns);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSpawn(FString CurrentLevel, int CurrentSpawn);

	UFUNCTION(BlueprintCallable)
	void SetMidpointCleared(FString CurrentLevel, bool MidPointCleared);

	UFUNCTION(BlueprintCallable)
	void SetDragonCoinsCollected(FString CurrentLevel, bool DragonCoinsCollected);

	UFUNCTION(BlueprintCallable)
	TArray<APlayerSpawn*> GetLevelSpawns(FString CurrentLevel);

	UFUNCTION(BlueprintCallable)
	int GetSpawnIndex(FString CurrentLevel);
	
	UFUNCTION(BlueprintCallable)
	bool GetMidpointCleared(FString CurrentLevel);

	UFUNCTION(BlueprintCallable)
	bool GetDragonCoinsCollected(FString CurrentLevel);

	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static ULevelSaveData* LoadLevelSaveData();

	UFUNCTION(BlueprintCallable, Category="Saving/Loading")
	static void SaveLevelSaveData(ULevelSaveData* In_SaveGame);
	
};



