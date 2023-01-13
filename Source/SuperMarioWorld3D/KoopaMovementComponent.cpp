// Fill out your copyright notice in the Description page of Project Settings.


#include "KoopaMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UKoopaMovementComponent::UKoopaMovementComponent()
{
}

void UKoopaMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	KoopaOwner = Cast<AKoopa>(GetOwner());

}

void UKoopaMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode) 
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Movement Mode"));
	}
}

UFUNCTION(BlueprintCallable) void UKoopaMovementComponent::EnterSlide()
{
	Velocity += UpdatedComponent->GetForwardVector() * Slide_EnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UKoopaMovementComponent::ExitSlide()
{
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, OldRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

void UKoopaMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME) 
	{
		return;
	}

	FHitResult SurfaceHit;
	bool surfaceCheck = GetSlideSurface(SurfaceHit);
	FVector DPMultiplier = FVector(0, 0, 1);
	double ImpactDP = FVector::DotProduct(SurfaceHit.ImpactNormal, DPMultiplier);
	float FloorDegrees = UKismetMathLibrary::Acos(ImpactDP);

	FString VelSize = FString::SanitizeFloat(Velocity.Size());
	FString MinSize = FString::SanitizeFloat(Slide_MinSpeed);
	if(FloorDegrees < 10 && Velocity.Size() < Slide_MinSpeed)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	
	
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity()) 
	{
		CalcVelocity(deltaTime, Slide_Friction, false, GetMaxBrakingDeceleration());
	}
	
	
	if (surfaceCheck) 
	{
		Velocity += Slide_GravityForce * FVector::DownVector * deltaTime * (UpdatedComponent->GetForwardVector() * 1400) - ((UpdatedComponent->GetForwardVector() * UpdatedComponent->GetForwardVector()) * 6.57);
	}
	else 
	{
		Velocity += Slide_FallingGravityForce * FVector::DownVector * deltaTime + (UpdatedComponent->GetForwardVector() * 14);
	}

	Iterations++;
	bJustTeleported = false;
	
	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	SafeMoveUpdatedComponent(Adjusted, OldRotation, true, Hit);

	if (Hit.Time < 1.f) 
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) && Velocity.SizeSquared() < Slide_MinSpeed)
	{
		ExitSlide();
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity()) 
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

}

bool UKoopaMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, KoopaOwner->GetIgnoreCharacterParams());
}

UFUNCTION(BlueprintPure) bool UKoopaMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

