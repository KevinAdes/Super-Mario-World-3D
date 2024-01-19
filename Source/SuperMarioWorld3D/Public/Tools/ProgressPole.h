// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProgressPole.generated.h"

class APlayerSpawn;
class USplineMeshComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTapeHit, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoalPassed, AActor*, OtherActor);

UCLASS(Blueprintable, BlueprintType)
class SUPERMARIOWORLD3D_API AProgressPole : public AActor
{
	GENERATED_BODY()

	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	//variables
	UPROPERTY(EditInstanceOnly)
	bool bMidPoint;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	APlayerSpawn* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PoleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* TapeMesh;
	
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineMeshComponent* Tape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Pole1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Pole2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* GoalCollision = nullptr;
public:
	//functions
	
	// Sets default values for this actor's properties
	AProgressPole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//delegates
	UFUNCTION()
	void OnTapeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGoalPassed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UPROPERTY(BlueprintAssignable)
	FTapeHit TapeHit;

	UPROPERTY(BlueprintAssignable)
	FGoalPassed GoalPassed;
};
