// Fill out your copyright notice in the Description page of Project Settings.


#include "Koopa.h"
#include "KoopaMovementComponent.h"

// Sets default values
AKoopa::AKoopa(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKoopaMovementComponent>(CharacterMovementComponentName))
{
	KoopaMovementComponent = Cast<UKoopaMovementComponent>(GetCharacterMovement());
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKoopa::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKoopa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKoopa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FCollisionQueryParams AKoopa::GetIgnoreCharacterParams() const 
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}
