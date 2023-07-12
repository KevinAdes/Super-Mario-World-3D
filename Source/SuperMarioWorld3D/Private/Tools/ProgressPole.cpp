// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ProgressPole.h"

#include "LevelSaveData.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"


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
	Tape->SetupAttachment(RootComponent);

	// Bind the OnComponentBeginOverlap event to the OnGoalTapeHit function
	Tape->OnComponentBeginOverlap.AddDynamic(this, &AProgressPole::OnTapeHit);
}

// Called when the game starts or when spawned
void AProgressPole::BeginPlay()
{
	Super::BeginPlay();
	//what a line
	if(ULevelSaveData::LoadLevelSaveData()->GetMidpointCleared(UGameplayStatics::GetCurrentLevelName(GetWorld())) && bMidPoint)
	{
		Tape->DestroyComponent();
	}
	
	if(IsValid(Tape) && IsValid(Pole1) && IsValid(Pole2))
	{
	
		FVector Direction = Pole2->GetComponentLocation() - Pole1->GetComponentLocation();

		// Normalize the direction vector
		Direction.Normalize();

		// Set the start and end points and tangents of the GoalTape
		const FVector StartPoint = Pole1->GetComponentLocation();
		const FVector StartTangent = Direction; //* SomeLength; // Adjust the length as needed
		const FVector EndPoint = Pole2->GetComponentLocation();
		const FVector EndTangent = -Direction;// * //SomeLength; //
	
		// Adjust the GoalTape position and scale based on the positions of Pole1 and Pole2
		Tape->SetStartAndEnd(StartPoint, StartTangent,EndPoint,EndTangent, true);
		Tape->SetStartScale(FVector2D(1.0f, 1.0f));
		Tape->SetEndScale(FVector2D(1.0f, 1.0f));	
	}
}

void AProgressPole::OnTapeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("TapeHit"))
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	ULevelSaveData* LevelSaveData = ULevelSaveData::LoadLevelSaveData();
	LevelSaveData->SetMidpointCleared(CurrentLevelName, true);
	LevelSaveData->SetCurrentSpawn(CurrentLevelName, SpawnPoint->Index);
	if(bMidPoint)
	{
		
	}
	else
	{
		
	}
	ULevelSaveData::SaveLevelSaveData(LevelSaveData);
	Tape->DestroyComponent();
}
// Called every frame
void AProgressPole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
