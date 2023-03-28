// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MarioMovementComponent.generated.h"

UCLASS()
class SUPERMARIOWORLD3D_API UMarioMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite)
	float XVel = 0;
	UPROPERTY(BlueprintReadWrite)
	float YVel = 0;
	UPROPERTY(BlueprintReadWrite)
	float maxXVel;
	UPROPERTY(BlueprintReadWrite)
	float accel;
	UPROPERTY(BlueprintReadWrite)
	float stoppingDecel;
	UPROPERTY(BlueprintReadWrite)
	float skiddingDecel;
	UPROPERTY(BlueprintReadWrite)
	float pMeter;
	UPROPERTY(BlueprintReadWrite)
	float gravity;

	UPROPERTY(BlueprintReadOnly)
	float pMin = 0;
	UPROPERTY(BlueprintReadOnly)
	float pMax = 112;
	UPROPERTY(BlueprintReadOnly)
	float maxSprintVel = 30;
	UPROPERTY(BlueprintReadOnly)
	float maxYVel = 40;
	UPROPERTY(BlueprintReadOnly)
	float forwardAirAccel = .9375;
	UPROPERTY(BlueprintReadOnly)
	float backwardAirAccel = 1.5625;
	UPROPERTY(BlueprintReadOnly)
	float runBackAirAccel = 3.125;


	UPROPERTY(BlueprintReadOnly)
	// 0 = up 45 degree slope; 1 = up 23 degree slope; 2 = flat, 11 degree slope; 3 = down 23 degree slope; 4 = down 45/67 degree slope
	TArray<float> MaxWalkVelocities = { 9.375, 10.625, 12.5, 15, 22.5 };

	UPROPERTY(BlueprintReadOnly)
	//0 = up 45 degree slope; 1 = up 23 degree slope; 2 = all else
	TArray<float> MaxRunVelocities = { 16.875, 19.375, 22.5 };


	UPROPERTY(BlueprintReadOnly)
	//0 = down 11 degree slope; 1 = down 23 degree slope; 2 = down 45 degree slope
	TArray<float> MaxSlidingVelocities = { 25, 27.5, 30 };
		

	UPROPERTY(BlueprintReadOnly)
	//based on Xvelocities, 0 = <5; 1= <10; 2= <15; 3= <20; 4= <25; 5= <3; 6 = 3;
	TArray<float> JumpVelocities = { 48.125,49.375,51.25,52.5,54.375,55.625,57.5 };

	UPROPERTY(BlueprintReadOnly)
	TArray<float> SpinJumpVelocities = { 44.375, 45.625, 46.875, 48.125, 49.375, 51.25, 52.5 };


	UPROPERTY(BlueprintReadOnly)
	//0 = up 45 degree slope; 1 = up 23 degree slope; 2 = all else
	TArray<float> Accelerations = { .625, .78125, .9375, 2.5 };

	UPROPERTY(BlueprintReadOnly)
	//0 = down 45 degree slope; 1 = down 23 degree slope; 2 = flat, 11 degree slope; 3 = up 23 degree slope; 4 = up 45 degree slope
	TArray<float> StoppingDecelerations = { .15625, .46875, .625, .9375, 1.25 };

	UPROPERTY(BlueprintReadOnly)
	//0 = down 45 degree slope; 1 = down 23 degree slope; 2 = flat, 11 degree slope; 3 = up 23 degree slope; 4 = up 45 degree slope
	TArray<float> WalkingSkidDecelerations = { 1.25, 1.40625, 1.5625, 1.71875, 1.875 };

	UPROPERTY(BlueprintReadOnly)
	//0 = down 45 degree slope; 1 = down 23 degree slope; 2 = flat, 11 degree slope; 3 = up 23 degree slope; 4 = up 45 degree slope
	TArray<float> RunningSkidDecelerations = { 2.5, 2.8125, 3.125, 3.4375, 3.75 };

	UPROPERTY(BlueprintReadOnly)
	//0 = down 11 degree slope; 1 = down 23 degree slope; 2 = down 45 degree slope
	TArray<float> SlidingAccelerations = { .3125, .625, .9375 };

	UPROPERTY(BlueprintReadOnly)
	//0 = b held; 1 = b not held
	TArray<float> Gravities = { 1.875, 3.75 };

public:
	UMarioMovementComponent();
	
	
};
