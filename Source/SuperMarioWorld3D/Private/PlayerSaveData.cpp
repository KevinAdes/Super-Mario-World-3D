// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveData.h"

#include "Kismet/GameplayStatics.h"

void UPlayerSaveData::SetSaveData(EPlayerSaveDataValue type, int32 value)
{
	SaveData.SetData(type, value);
	
}

int32 UPlayerSaveData::GetSaveData(EPlayerSaveDataValue type)
{
	return SaveData.GetData(type);
}

UPlayerSaveData* UPlayerSaveData::LoadPlayerSaveData()
{
	if(USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot("PlayerSaveData", 0))
	{
		if(UPlayerSaveData* PlayerSaveGame = Cast<UPlayerSaveData>(SaveGame))
		{
			return PlayerSaveGame;
		}
	}
	UPlayerSaveData* PlayerSaveGame = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, "PlayerSaveData", 0);
	return PlayerSaveGame;
}

void UPlayerSaveData::SavePlayerSaveData(UPlayerSaveData* In_SaveGame)
{
	if(In_SaveGame)
	{
		UGameplayStatics::SaveGameToSlot(In_SaveGame, "PlayerSaveData", 0);
	}
}
