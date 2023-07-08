// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawn.generated.h"


UENUM(BlueprintType)
enum class ESpawnType : uint8
{
	ST_Regular			UMETA(DisplayName = "Regular"),
	ST_PipeUp			UMETA(DisplayName = "Pipe Up"),
	ST_PipeDown			UMETA(DisplayName = "Pipe Down"),
	ST_PipeForward		UMETA(DisplayName = "Pipe Forward"),
	ST_PipeLaunch		UMETA(DisplayName = "Pipe Launch"),
};

UCLASS()
class SUPERMARIOWORLD3D_API APlayerSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSpawn();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector SpawnPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator SpawnDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ESpawnType SpawnType;

	//todo: move all this level blueprint shit into cpp
	UPROPERTY(BlueprintReadWrite);
	int32 Index;
};
