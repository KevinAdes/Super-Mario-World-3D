// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataLibrary.generated.h"
/**
 * 
 */
class SUPERMARIOWORLD3D_API DataLibrary
{
public:
	DataLibrary();
	~DataLibrary();
};

USTRUCT(BlueprintType)
struct SUPERMARIOWORLD3D_API FMarioPhysicsProperties : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NaturalVelocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkAccel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunAccel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Decel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSkidDecel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSkidDecel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideDecel = 0;

};
