// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProgressPole.generated.h"

class APlayerSpawn;
class USplineMeshComponent;
UCLASS(Blueprintable, BlueprintType)
class SUPERMARIOWORLD3D_API AProgressPole : public AActor
{
	GENERATED_BODY()

	
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineMeshComponent* Tape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Pole1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Pole2;

	UFUNCTION()
	void OnTapeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:

	UPROPERTY(EditInstanceOnly)
	bool bMidPoint;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	APlayerSpawn* SpawnPoint;
	// Sets default values for this actor's properties
	AProgressPole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
