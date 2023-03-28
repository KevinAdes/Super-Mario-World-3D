
// Fill out your copyright notice in the Description page of Project Settings.

#include "Mario.h"
#include "MarioMovementComponent.h"

// Sets default values
AMario::AMario(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMarioMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	MarioMovementComponent = Cast<UMarioMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void AMario::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMario::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

