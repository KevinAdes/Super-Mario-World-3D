
// Fill out your copyright notice in the Description page of Project Settings.

#include "Mario.h"

#include "LevelSaveData.h"
#include "MarioMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "KismetTraceUtils.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/PowerUp.h"
#include "Kismet/KismetMathLibrary.h"
#include "SuperMarioWorld3D/GameMode/SMWPlayerState.h"

#pragma region lifetime
// Sets default values
AMario::AMario(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	MarioMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MarioMesh->SetupAttachment(Root);
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	Collision->SetupAttachment(Root);
	HoldBox = CreateDefaultSubobject<UBoxComponent>("Hold Box");
	HoldBox->SetupAttachment(Root);
	StompBox = CreateDefaultSubobject<UBoxComponent>("Stomp Box");
	StompBox->SetupAttachment(Root);
	EnhancedInputComponent = CreateDefaultSubobject<UEnhancedInputComponent>("Input");
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(Root);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("Shadow");
	DecalComponent->SetupAttachment(Root);
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>("PostProcess");	
}

void AMario::QuickSetPowerState_Implementation(int32 PowerState)
{
	power_state = PowerState;
}

void AMario::SetPowerState_Implementation(int32 PowerState)
{
	power_state = PowerState;
}

// Called when the game starts or when spawned
void AMario::BeginPlay()
{
	Super::BeginPlay();
	APlayerState* MyPlayerState = GetPlayerState();
	SMW_player_state = Cast<ASMWPlayerState>(MyPlayerState);

	if (!SMW_player_state)
	{
		return;
	}

	QuickSetPowerState(SMW_player_state->GetPowerState());
	BindHudEvents();

	APlayerSpawn* PlayerSpawn = GetCurrentSpawnPointFromLevelSaveData();
	if (!IsValid(PlayerSpawn))
	{
		PlayerSpawn = FindDefaultSpawnInLevel();
	}
	SendMarioToSpawnPoint(PlayerSpawn);

}

void AMario::BindHudEvents()
{
	LevelHUD = NewObject<ULevelHUD>();
	LevelHUD->AddToViewport();
	
}

void AMario::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (auto PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SetupPlayerInputComponent();

}


// Called to bind functionality to input
void AMario::SetupPlayerInputComponent()
{
	//Left Analog stick
	EnhancedInputComponent->BindAction(InputHorizontalMove, ETriggerEvent::Triggered, this, &AMario::ApplyHorizontalInput);
	InputHorizontalMoveValueBinding = &EnhancedInputComponent->BindActionValue(InputHorizontalMove);
	
	//Face Buttons
	EnhancedInputComponent->BindAction(InputJump, ETriggerEvent::Started, this, &AMario::Jump);
	EnhancedInputComponent->BindAction(InputSpinJump, ETriggerEvent::Started, this, &AMario::SpinJump);
	EnhancedInputComponent->BindAction(InputSpecial, ETriggerEvent::Started, this, &AMario::SpecialButtonPressed);
	EnhancedInputComponent->BindAction(InputSpecial, ETriggerEvent::Completed, this, &AMario::SpecialButtonReleased);
	InputJumpValueBinding = &EnhancedInputComponent->BindActionValue(InputJump);
	InputSpinJumpValueBinding = &EnhancedInputComponent->BindActionValue(InputSpinJump);
	InputSpecialValueBinding = &EnhancedInputComponent->BindActionValue(InputSpecial);
	
	//Shoulder Buttons
	EnhancedInputComponent->BindAction(InputCrouch, ETriggerEvent::Started, this, &AMario::SMW_Crouch);
	EnhancedInputComponent->BindAction(InputCrouch, ETriggerEvent::Completed, this, &AMario::SMW_UnCrouch);
	EnhancedInputComponent->BindAction(InputLookUp, ETriggerEvent::Started, this, &AMario::LookUp);
	EnhancedInputComponent->BindAction(InputLookUp, ETriggerEvent::Completed, this, &AMario::UnLookUp);
}

bool AMario::IsPlayerAddingMovement()
{
	return InputHorizontalMoveValueBinding->GetValue().Get<FVector2d>().Length() > .25f;
}

bool AMario::IsMarioMoving()
{
	return HorizontalVelocity.Length() > 0;
}

// Called every frame
void AMario::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::IsGamePaused(this))
	{
		return;
	}

	SetPhysicsValues();
	SetJumpVelocities();
	UpdatePMeter();

	AdjustVelocityForSlope();

	if ((!IsPlayerAddingMovement() && IsMarioMoving()) || (MarioState == EMarioStates::Crouched && IsMarioGrounded()))
	{
		IdleDecellerate();
	}
}

void AMario::EndLevelSequence()
{
}


void AMario::Destroyed()
{
	Super::Destroyed();
}

#pragma endregion

#pragma region physics
void AMario::SetPhysicsValues()
{
	FHitResult ForwardHitResult;
	bool useUpHillPhysics;
	TraceForward(ForwardHitResult, useUpHillPhysics);
	float FloorAngle = GetFloorAngle();

	if (FMath::IsNearlyEqual(FloorAngle, 0, 6))
	{
		useUpHillPhysics = true;
	}

	if (InputHorizontalMoveValueBinding->GetValue().Get<FVector2D>().Length() < .25)
	{
		useUpHillPhysics = false; 
	}
		
	TArray<FMarioPhysicsProperties*> PhysicsPropertiesArray;
	PhysicsPropertiesTable->GetAllRows<FMarioPhysicsProperties>("", PhysicsPropertiesArray);

	for (int i = 0; i < PhysicsPropertiesArray.Num(); i++)
	{
		FMarioPhysicsProperties* Properties = PhysicsPropertiesArray[i];
		if (FMath::IsNearlyEqual(FloorAngle, Properties->FloorAngle, Properties->AngleTolerance) && useUpHillPhysics == Properties->UpHill)
		{
			PhysicsProperties = Properties;
			return;
		}
	}
}

void AMario::UpdateHorizontalVelocity(FVector2d NewXYVelocity)
{
	HorizontalVelocity = NewXYVelocity;
}

void AMario::UpdateVerticalVelocity(float NewZVelocity)
{
	VerticalVelocity = NewZVelocity;
}

void AMario::AccelerateVector2d(FVector2d& Vector, float Acceleration)
{
	//chat... is this real?
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float InitialMagnintude = Vector.Length();
	float AcceleratedMagnitude = InitialMagnintude + Acceleration;
	float AdjustmentFactor = AcceleratedMagnitude/InitialMagnintude;
	Vector = Vector * AdjustmentFactor;
}

void AMario::AdjustVelocityForSlope()
{
	float FloorAngle = GetFloorAngle();
	if (FloorAngle == 0.0f)
	{
		return;
	}
	
	float verticalProportion = FloorAngle/45;

	FHitResult ForwardHitResult;
	bool ForwardHitSuccess;
	TraceForward(ForwardHitResult, ForwardHitSuccess);
	bool MovingUpHill = ForwardHitSuccess && IsPlayerAddingMovement();
	int32 Direction = MovingUpHill ?  1 : -1;
	
	float newVerticalVelocity = HorizontalVelocity.Length() * verticalProportion * Direction;
	UpdateVerticalVelocity(newVerticalVelocity);
	
	FHitResult DownwardHitResult;
	bool DownHitSuccess;
	TraceDown(DownwardHitResult, DownHitSuccess);
	
	FVector2d SlopeVector = FVector2d(DownwardHitResult.Normal.X, DownwardHitResult.Normal.Y);
	SlopeVector.Normalize();
	
	UpdateHorizontalVelocity(SlopeVector * PhysicsProperties->NaturalVelocity * PHYS_FACTOR);
}

void AMario::IdleDecellerate()
{
	AccelerateVector2d(HorizontalVelocity, PhysicsProperties->Decel);
	if (HorizontalVelocity.Length() <= MIN_WALK_SPEED)
	{
		UpdateHorizontalVelocity(FVector2d::Zero());
		OnMovementStopped.Broadcast();
	}
}

void AMario::ApplyGravity()
{
	bool JumpHeld = InputJumpValueBinding->GetValue().Get<bool>() || InputSpinJumpValueBinding->GetValue().Get<bool>();

	bool hasMarioReachedTerminalVelocity = VerticalVelocity <= TERMINAL_VELOCITY;

	float ForceOfGravity = JumpHeld ? JUMP_HELD_GRAVITY : DEFAULT_GRAVITY;
	float AdjustedForceOfGravity = ForceOfGravity * PHYS_FACTOR * GetWorld()->DeltaRealTimeSeconds;

	float newVerticalVelocity = hasMarioReachedTerminalVelocity? TERMINAL_VELOCITY : VerticalVelocity - AdjustedForceOfGravity;

	UpdateVerticalVelocity(newVerticalVelocity);

	bool isMarioFalling = newVerticalVelocity < 0;
	bool CrouchingOrSpinJumping = crouched || MarioState == SpinJumping;
	bool PNotFull = p_meter < 112;
	bool shouldTriggerFallAnimation = isMarioFalling && !CrouchingOrSpinJumping && PNotFull;
	if (shouldTriggerFallAnimation)
	{
		Anim_Fall();
	}
}

void AMario::UpdateCharacterLocation()
{
	FVector3d TotalVelocity = FVector3d(HorizontalVelocity.X, HorizontalVelocity.Y, VerticalVelocity);
	FHitResult* Result = new FHitResult;
	bool success = SetActorLocation(GetActorLocation()+TotalVelocity, true, Result);
	if (!success)
	{
		SetActorLocation(Result->Location);
	}
	FHitResult FloorHitResult;
	bool FloorHitSuccess;
	TraceDown(FloorHitResult, FloorHitSuccess);
	if (!FloorHitSuccess && !IsJumping())
	{
		LedgeWalkOff();
	}
}

void AMario::UpdateCameraLocation()
{
	
}

void AMario::RotateCharacter()
{
}

void AMario::TraceForward(FHitResult& HitResult, bool& HitDetected)
{
	FVector3d StartVector = GetActorLocation();
	FVector3d EndVector = GetActorLocation() + (GetActorForwardVector() * 80);
	EDrawDebugTrace::Type DrawDebugTrace = DrawDebugTraces ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor HitColor = FLinearColor::Green;
	float DrawTime = .5f;
	DrawDebugLineTraceSingle(GetWorld(), StartVector, EndVector, DrawDebugTrace, HitDetected, HitResult, TraceColor, HitColor, DrawTime);
}

void AMario::TraceDown(FHitResult& HitResult, bool& HitDetected)
{
	FVector3d StartVector = GetActorLocation();
	FVector3d EndVector = GetActorLocation() + FVector3d(0,0, -100);
	EDrawDebugTrace::Type DrawDebugTrace = DrawDebugTraces ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor HitColor = FLinearColor::Green;
	float DrawTime = .5f;
	DrawDebugSphereTraceSingle(GetWorld(), StartVector, EndVector, 40, DrawDebugTrace, HitDetected, HitResult, TraceColor, HitColor, DrawTime);
}

float AMario::GetFloorAngle()
{
	FHitResult FloorHitResult;
	bool HitDetected;
	
	TraceDown(FloorHitResult, HitDetected);

	if (!HitDetected || FloorHitResult.Distance <= .1f)
	{
		return 0.0f; 
	}

	FVector3d HitNormal = FloorHitResult.ImpactNormal;
	float NormalCoefficient = HitNormal.Dot(FVector3d::UpVector);
	float FloorAngle = acosf(NormalCoefficient);

	/*
	if (FMath::IsNearlyEqual(FloorAngle, 90, 2.f))
	{
		FloorAngle = 0;
	}
	*/
	
	return FloorAngle;
}

bool AMario::IsMarioGrounded()
{
	FHitResult HitResult;
	bool HitSuccess;
	TraceDown(HitResult, HitSuccess);
	if (!HitSuccess)
	{
		return false;
	}
	return HitResult.Distance <= .1f;
}

#pragma endregion

#pragma region Movement and controls

void AMario::ApplyHorizontalInput(const FInputActionValue& InputActionValue)
{
	if (MarioState == EMarioStates::LookingUp)
	{
		return;
	}
	//TODO add check for is grounded
	if (MarioState == EMarioStates::Crouched)
	{
		return;
	}

	if (HorizontalVelocity.Length() <= (skidding ? 0.25f : 0.0f))
	{
		if (skidding)
		{
			EndSkid();
		}

		FVector2d NewVelocity;


		UpdateHorizontalVelocity(NewVelocity);
		
	}
}

void AMario::SMW_Crouch()
{
	//is this necessary?
	crouched = true;

	// if not moving on ground, return;
	
	if (IsMarioOnPipe())
	{
		Anim_Crouch();
		//warp
		return;
	}

	MarioState = EMarioStates::Crouched;
	
	if (GetFloorAngle() >= 11.0f)
	{
		Anim_Slide();
		OnMovementStopped.AddUniqueDynamic(this, &AMario::SMW_Crouch);
	}
	else
	{
		Anim_Crouch();
	}
	SpawnDustParticle();
}

void AMario::SMW_UnCrouch()
{
	crouched = false;
	OnMovementStopped.RemoveDynamic(this, &AMario::SMW_Crouch);
	
	if (MarioState != EMarioStates::Crouched)
	{
		return;
	}
	
	if (HorizontalVelocity.Length() >= 10)
	{
		Anim_Walk();
		MarioState = EMarioStates::Moving;
		return;
	}
	
	Anim_Idle();
	MarioState = EMarioStates::Idle;
}

void AMario::LookUp()
{
	looking_up = true;
	
	if (HorizontalVelocity.Length() > .1 || IsJumping())
	{
		OnMovementStopped.AddUniqueDynamic(this, &AMario::LookUp);
		return;
	}

	MarioState = EMarioStates::LookingUp;
	Anim_Lookup();
}

void AMario::UnLookUp()
{
	looking_up = false;
	OnMovementStopped.RemoveDynamic(this, &AMario::LookUp);

	if (MarioState != EMarioStates::LookingUp)
	{
		return;
	}

	MarioState = EMarioStates::Idle;
	Anim_Idle();
}

void AMario::BeginSkid()
{
	skidding = true;
	if (skid_timer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(skid_timer);
	}
	GetWorldTimerManager().SetTimer(skid_timer, this, &AMario::EndSkid, .2);
	if (!IsJumping())
	{
		SpawnDustParticle();
		Anim_Skid();
	}
}
void AMario::EndSkid()
{
	if (skidding)
	{
		UpdateHorizontalVelocity(FVector2d(0.0,0.0));
	}
	skidding = false;
}

void AMario::LedgeWalkOff()
{
	
}

void AMario::Jump()
{
	if (IsJumping())
	{
		GetWorldTimerManager().SetTimer(jump_buffer_timer, jump_buffer_length, false, 0);
		return;
	}
	if (MarioState != Crouched)
	{
		//SetStandingHeight
	}

	//if this results in weird behavior, maybe just set skidding to false
	EndSkid();
	
	gravity_enabled = true;
	MarioState = NormalJumping;
	UpdateVerticalVelocity(JumpVelocities->JumpVelocity);

	//TODO: understand the real logic behind this
	if (HorizontalVelocity.Length() > PhysicsProperties->MaxWalkVelocity)
	{
		float JumpDelay = HorizontalVelocity.Length()/100;
		GetWorldTimerManager().SetTimer(delayed_jump_anim_timer, this, &AMario::Anim_Jump, JumpDelay);
	}
	Anim_Jump();
}


void AMario::SpinJump()
{
	if (IsJumping())
	{
		GetWorldTimerManager().SetTimer(spin_jump_buffer_timer, jump_buffer_length, false, 0);
		return;
	}
	if (holding)
	{
		Jump();
		return;
	}
	gravity_enabled = true;

	MarioState = EMarioStates::SpinJumping;
	Anim_Idle();

	UpdateVerticalVelocity(JumpVelocities->SpinJumpVelocity);
	//spin rotation

	if (power_state == 2)
	{
		GetWorldTimerManager().SetTimer(spin_fire_timer, this, &AMario::ThrowFireball, .2, true);
	}
}

void AMario::OnLanding()
{
	ResetComboCount();
	UpdateVerticalVelocity(0);
	GetWorldTimerManager().ClearTimer(spin_fire_timer);
	if (MarioState == EMarioStates::SpinJumping)
	{
		//update rotation
	}
	skidding = false;
	gravity_enabled = false;

	if (GetWorldTimerManager().IsTimerActive(jump_buffer_timer))
	{
		Jump();
		return;
	}

	if (GetWorldTimerManager().IsTimerActive(spin_jump_buffer_timer))
	{
		SpinJump();
		return;
	}
	
	if (crouched)
	{
		SMW_Crouch();
		return;
	}
	
	if (HorizontalVelocity.Length() >= PhysicsProperties->WalkAccel)
	{
		Anim_Walk();
		MarioState = EMarioStates::Moving;
		return;
	}
	
	Anim_Idle();
	MarioState = EMarioStates::Idle;
}

bool AMario::IsJumping()
{
	return MarioState == EMarioStates::NormalJumping || MarioState == EMarioStates::SpinJumping;
}

void AMario::SpecialButtonPressed()
{
	if (power_state == 2)
	{
		ThrowFireball();
	}
}

void AMario::ThrowFireball()
{
	fire_balls.RemoveAll([](const UObject* FireBall)
	{
		return FireBall == nullptr;
	});

	if (fire_balls.Num() >= 2)
	{
		return;
	}

	if (MarioState != EMarioStates::SpinJumping)
	{
		GetWorldTimerManager().ClearTimer(anim_freeze_timer);
		anim_frozen = false;
		Anim_ThrowFireBall();
		anim_frozen = true;
		GetWorldTimerManager().SetTimer(anim_freeze_timer, this, &AMario::UnfreezeAnimation, .2f);
	}

	SpawnFireBall();  
}

void AMario::SpecialButtonReleased()
{
	if (holding)
	{
		KickHeldItem();
	}
}

void AMario::Pause()
{
}

void AMario::ConfirmMessageBlock()
{
}

void AMario::KickHeldItem()
{
}

void AMario::GetStoredPowerUp()
{
	int32 StoredItemPowerValue = SMW_player_state->GetStoredPowerUpValue();
	if (StoredItemPowerValue <= 0)
	{
		return;
	}
	
	TArray<FPowerUpData*> PowerUpDataArray;
	PowerUpsTable->GetAllRows<FPowerUpData>("", PowerUpDataArray);

	for (int i = 0; i < PowerUpDataArray.Num(); i++)
	{
		FPowerUpData* PowerUpData = PowerUpDataArray[i];
		if (PowerUpData->PowerValue == StoredItemPowerValue)
		{
			FVector SpawnLocation = ((CameraComponent->GetForwardVector() + (CameraComponent->GetUpVector() * .45)) * 2000) + CameraComponent->GetComponentLocation();
			FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, CameraComponent->GetComponentLocation());
			SpawnRotation = FRotator(0, SpawnRotation.Yaw, 0);
			FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
			UPowerUp* PowerUp = GetWorld()->SpawnActor<UPowerUp>(PowerUpData->PowerUp.GetClass(), SpawnLocation, SpawnRotation, ActorSpawnParameters);
			PowerUp->ItemBoxFall();
			return;
		}
	}
	
	
}

void AMario::SetJumpVelocities()
{
}

void AMario::UpdatePMeter()
{
}

void AMario::TakeDamage(bool instantDeath)
{
	if (instantDeath)
	{
		Kill();
		return;
	}
	SetPowerState(0);
	GetStoredPowerUp();
	
}

void AMario::Kill()
{
}

void AMario::TimeOutKill()
{
}

void AMario::PitKill()
{
}

void AMario::DecrementTime()
{
}

void AMario::PauseTimer()
{
}

void AMario::ResumeTimer()
{
}

#pragma endregion

#pragma region warp

APlayerSpawn* AMario::GetCurrentSpawnPointFromLevelSaveData() const
{
	ULevelSaveData* LevelSaveData = ULevelSaveData::LoadLevelSaveData();
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelSaveData->SavedLevelMap.Contains(LevelName))
	{
		TArray<APlayerSpawn*> LevelSpawns = LevelSaveData->GetLevelSpawns(LevelName);
		int32 SpawnIndex = LevelSaveData->GetSpawnIndex(LevelName);
		return LevelSpawns[SpawnIndex];
	}
	return nullptr;
}

APlayerSpawn* AMario::FindDefaultSpawnInLevel() const
{
	TArray<AActor*> DefaultLevelSpawns;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerSpawn::StaticClass(), FName("Default"), DefaultLevelSpawns);
	checkf(!DefaultLevelSpawns.IsEmpty(), TEXT("No default spawnpoint found. Add Default spawnpoint to level"));
	checkf(!(DefaultLevelSpawns.Num() > 1), TEXT("More than one default level spawnpoint found. only set one spawnpoint as default"))

	APlayerSpawn* ret = Cast<APlayerSpawn>(DefaultLevelSpawns[0]);
	return ret;
}

void AMario::SendMarioToSpawnPoint(APlayerSpawn* PlayerSpawn)
{
	
}

bool AMario::IsMarioOnPipe()
{
	return false;
}

#pragma endregion

void AMario::ResetComboCount()
{
}


void AMario::SpawnFireBall()
{
}

#pragma region animation

void AMario::UnfreezeAnimation()
{
}

void AMario::FreezeAnimation()
{
}

#pragma endregion