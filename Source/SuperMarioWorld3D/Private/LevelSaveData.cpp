// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSaveData.h"

#include "Kismet/GameplayStatics.h"

ULevelSaveData::ULevelSaveData()
{
	AddSlot("Yoshis_Island_1");
}

void ULevelSaveData::AddSlot(FString CurrentLevel)
{
	if (!SavedLevelMap.Contains(CurrentLevel))
	{
		SavedLevelMap.Add(CurrentLevel);
	}
	SaveLevelSaveData(this);
}

void ULevelSaveData::SetLevelSpawns(FString CurrentLevel, TArray<APlayerSpawn*> Spawns)
{
	SavedLevelMap.Find(CurrentLevel)->SetSpawnLocations(Spawns);
}

void ULevelSaveData::SetCurrentSpawn(FString CurrentLevel, int CurrentSpawn)
{
	SavedLevelMap.Find(CurrentLevel)->SetSpawnIndex(CurrentSpawn);
}

void ULevelSaveData::SetMidpointCleared(FString CurrentLevel, bool MidPointCleared)
{
	SavedLevelMap.Find(CurrentLevel)->SetMidPoleCleared(MidPointCleared);
}

void ULevelSaveData::SetDragonCoinsCollected(FString CurrentLevel, bool DragonCoinsCollected)
{
	SavedLevelMap.Find(CurrentLevel)->SetDragonCoinsCollected(DragonCoinsCollected);
}

TArray<APlayerSpawn*> ULevelSaveData::GetLevelSpawns(FString CurrentLevel)
{
	return SavedLevelMap.Find(CurrentLevel)->GetSpawnLocations();
}

int ULevelSaveData::GetSpawnIndex(FString CurrentLevel)
{
	return SavedLevelMap.Find(CurrentLevel)->GetSpawnIndex();
}

bool ULevelSaveData::GetMidpointCleared(FString CurrentLevel)
{
	return SavedLevelMap.Find(CurrentLevel)->GetMidPoleCleared();
}

bool ULevelSaveData::GetDragonCoinsCollected(FString CurrentLevel)
{
	return SavedLevelMap.Find(CurrentLevel)->GetDragonCoinsCollected();
}

ULevelSaveData* ULevelSaveData::LoadLevelSaveData()
{
	if(USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot("LevelSaveData", 0))
	{
		if(ULevelSaveData* LevelSaveGame = Cast<ULevelSaveData>(SaveGame))
		{
			return LevelSaveGame;
		}
	}
	ULevelSaveData* LevelSaveGame = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(LevelSaveGame, "LevelSaveData", 0);
	return LevelSaveGame;
}

void ULevelSaveData::SaveLevelSaveData(ULevelSaveData* In_SaveGame)
{
	if(In_SaveGame)
	{
		UGameplayStatics::SaveGameToSlot(In_SaveGame, "LevelSaveData", 0);
	}
}

