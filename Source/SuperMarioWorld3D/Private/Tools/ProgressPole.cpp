// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ProgressPole.h"

#include "LevelSaveData.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AProgressPole::AProgressPole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach Pole1 component
	Pole1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole1"));
	RootComponent = Pole1;

	// Create and attach Pole2 component
	Pole2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole2"));
	Pole2->SetupAttachment(RootComponent);

	// Create and attach GoalTape component
	Tape = CreateDefaultSubobject<USplineMeshComponent>(TEXT("GoalTape"));
	Tape->SetupAttachment(Pole1);

	// Bind the OnComponentBeginOverlap event to the OnGoalTapeHit function
	Tape->OnComponentBeginOverlap.AddDynamic(this, &AProgressPole::OnTapeHit);

	if (!bMidPoint)
	{
		GoalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalCollision"));
		GoalCollision->SetupAttachment(RootComponent);
		GoalCollision->OnComponentBeginOverlap.AddDynamic(this, &AProgressPole::OnGoalPassed);
	}
}

void AProgressPole::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#pragma region ValidityChecks
	if(!IsValid(Pole1))
	{
		//NoPole?
		return;
	}
	if(!IsValid(Pole2))
	{
		//NoPole2?
		return;
	}
	if(!IsValid(Tape))
	{
		//NoTape??
		return;
	}
	if(!IsValid(PoleMesh))
	{
		//NoPoleMesh???
		return;
	}
	if(!IsValid(TapeMesh))
	{
		//NoTapeMesh???
		return;
	}
//No Bitches??????
#pragma endregion 
	FVector Direction = Pole2->GetComponentLocation() - Pole1->GetComponentLocation();

	// Normalize the direction vector
	Direction.Normalize();

	// Set the start and end points and tangents of the GoalTape
	const FVector StartPoint = Pole1->GetComponentLocation();
	const FVector StartTangent = Direction; //* SomeLength; // Adjust the length as needed
	const FVector EndPoint = Pole2->GetComponentLocation();
	const FVector EndTangent = Direction;// * //SomeLength; //

	// Adjust the GoalTape position and scale based on the positions of Pole1 and Pole2
	Tape->SetStartAndEnd(StartPoint, StartTangent,EndPoint,EndTangent, true);
	Tape->SetStartScale(FVector2D(1.0f, 1.0f));
	Tape->SetEndScale(FVector2D(1.0f, 1.0f));

	Pole1->SetStaticMesh(PoleMesh);
	Pole2->SetStaticMesh(PoleMesh);
	Tape->SetStaticMesh(TapeMesh);

	if (!bMidPoint)
	{
		GoalCollision->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(StartPoint, EndPoint));
		FVector temp = (StartPoint + EndPoint)/2;
		GoalCollision->SetWorldLocation(FVector(temp.X, temp.Y, StartPoint.Z+ 720));
		GoalCollision->SetBoxExtent(FVector(UKismetMathLibrary::Vector_Distance(StartPoint, EndPoint)/2, 2, 720));
	}
	
}

// Called when the game starts or when spawned
void AProgressPole::BeginPlay()
{
	Super::BeginPlay();
	ULevelSaveData* LvlData = ULevelSaveData::LoadLevelSaveData();

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (!LvlData->SavedLevelMap.Find(CurrentLevelName))
	{
		
		ULevelSaveData::SaveLevelSaveData(LvlData);
		return;
	}
	if (LvlData->GetMidpointCleared(CurrentLevelName) && bMidPoint)
	{
		Tape->DestroyComponent();
	}
	
	ULevelSaveData::SaveLevelSaveData(LvlData);
}


void AProgressPole::OnTapeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Tape->DestroyComponent();
	TapeHit.Broadcast(OtherActor);
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	ULevelSaveData* LevelSaveData = ULevelSaveData::LoadLevelSaveData();
	if(bMidPoint)
	{
		LevelSaveData->SetMidpointCleared(CurrentLevelName, true);
		LevelSaveData->SetCurrentSpawn(CurrentLevelName, SpawnPoint->Index);
	}
	else
	{
		LevelSaveData->SetMidpointCleared(CurrentLevelName, false);
		LevelSaveData->SetCurrentSpawn(CurrentLevelName, 0);
	}
	ULevelSaveData::SaveLevelSaveData(LevelSaveData);
}


void AProgressPole::OnGoalPassed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GoalPassed.Broadcast(OtherActor);

	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	ULevelSaveData* LevelSaveData = ULevelSaveData::LoadLevelSaveData();
	
	LevelSaveData->SetMidpointCleared(CurrentLevelName, false);
	LevelSaveData->SetCurrentSpawn(CurrentLevelName, 0);
	
	ULevelSaveData::SaveLevelSaveData(LevelSaveData);
}

