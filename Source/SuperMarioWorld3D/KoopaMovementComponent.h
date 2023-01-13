// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Koopa.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KoopaMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode 
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName = "Slide"),
	CMove_Max		UMETA(Hidden),
};

UCLASS()
class SUPERMARIOWORLD3D_API UKoopaMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


		//Parameters
	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 500;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 2000;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;
	UPROPERTY(EditDefaultsOnly) float Slide_FallingGravityForce = 10000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.5;

	UPROPERTY(Transient) AKoopa* KoopaOwner;

public:
	UKoopaMovementComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void PhysCustom(float deltaTime, int32 Iterations);


private:
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

public:
	UFUNCTION(BlueprintCallable) void EnterSlide();
	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
};
