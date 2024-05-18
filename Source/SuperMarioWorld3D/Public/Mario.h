// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PlayerSpawn.h"
#include "GameFramework/Character.h"
#include "Libraries/DataLibrary.h"
#include "UI/LevelHUD.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Components/PostProcessComponent.h"
#include "Components/DecalComponent.h"
#include "Mario.generated.h"

class UMarioMovementComponent;
class ASMWPlayerState;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;

enum EMarioStates
{
	Idle,
	Moving,
	Crouched,
	LookingUp,
	NormalJumping,
	SpinJumping,
};

USTRUCT(BlueprintType)
struct SUPERMARIOWORLD3D_API FMarioPhysicsProperties : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloorAngle = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngleTolerance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UpHill = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NaturalVelocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideMaxVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkAccel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunAccel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Decel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSkidDecel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSkidDecel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideDecel = 0;

};


USTRUCT(BlueprintType)
struct SUPERMARIOWORLD3D_API FMarioJumpVelocities : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalSpeedRequirement = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpVelocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpinJumpVelocity = 0;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementStopped);

UCLASS()
class SUPERMARIOWORLD3D_API AMario : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MarioMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> HoldBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> StompBox;
	
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	TObjectPtr<UMarioMovementComponent> MarioMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> DecalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPostProcessComponent> PostProcessComponent;
	
private:
	AMario(const FObjectInitializer& ObjectInitializer);



public:
#pragma region PowerupsAndItems
	UFUNCTION(BlueprintNativeEvent)
	void QuickSetPowerState(int32 PowerState);

	UFUNCTION(BlueprintNativeEvent)
	void SetPowerState(int32 PowerState);

	void SpawnFireBall();

	void KickHeldItem();

	void GetStoredPowerUp();
#pragma endregion

#pragma region Initialization
	void BindHudEvents();

	APlayerSpawn* GetCurrentSpawnPointFromLevelSaveData() const;

	APlayerSpawn* FindDefaultSpawnInLevel() const;
	
	UFUNCTION()
	void SendMarioToSpawnPoint(APlayerSpawn* PlayerSpawn);
	
#pragma endregion

#pragma region Physics
	void SetJumpVelocities();
	
	void SetPhysicsValues();

	void UpdatePMeter();
	
	void UpdateHorizontalVelocity(FVector2d NewXYVelocity);

	void UpdateVerticalVelocity(float NewZVelocity);

	void AccelerateVector2d(FVector2d& Vector, float Acceleration);
	
	void AdjustVelocityForSlope();

	void IdleDecellerate();

	void ApplyGravity();

	void UpdateCharacterLocation();

	void UpdateCameraLocation();

	void RotateCharacter();

	void TraceForward(FHitResult& HitResult, bool& HitDetected);
	
	void TraceDown(FHitResult& HitResult, bool& HitDetected);
	
	float GetFloorAngle();

	bool IsMarioGrounded();
#pragma endregion


	
protected:

#pragma region controls

	void Pause();

	void EndLevelSequence();

	void ConfirmMessageBlock();



#pragma endregion

#pragma region movement
	
	void ApplyHorizontalInput(const FInputActionValue& InputActionValue);
	
	void Jump();

	void SpinJump();

	void OnLanding();

	bool IsJumping();

	UFUNCTION()
	void SMW_Crouch();

	void SMW_UnCrouch();

	UFUNCTION()
	void LookUp();

	void UnLookUp();

	void BeginSkid();
	
	void EndSkid();

	void LedgeWalkOff();
	
#pragma endregion
	
#pragma region lifetime
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void Destroyed() override;

	void TakeDamage(bool instantDeath);
	
	void Kill();

	void TimeOutKill();

	void PitKill();
	
#pragma endregion

#pragma region Animations

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Idle();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Walk();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_SpinRotation();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Hold();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Jump();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Fall();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Lookup();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Crouch();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Slide();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_Skid();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_StopWalking();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_SetWalkFrame();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_IncrementWalkFrameIndex();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_QueueNextStep();

	UFUNCTION(BlueprintImplementableEvent)
	void Anim_ThrowFireBall();

	UFUNCTION()
	void UnfreezeAnimation();

	UFUNCTION()
	void FreezeAnimation();

	bool anim_frozen = false;
	
#pragma endregion
		
	void DecrementTime();

	void PauseTimer();

	void ResumeTimer();

	void ResetComboCount();

		//add warp pipe reference parameter
	bool IsMarioOnPipe();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDustParticle();

	UFUNCTION()
	void SpecialButtonPressed();

	UFUNCTION()
	void SpecialButtonReleased();
	
	void ThrowFireball();
	
public:	
	// Called every frame

	// Called to bind functionality to input
	void SetupPlayerInputComponent();
	bool IsPlayerAddingMovement();
	bool IsMarioMoving();

	UPROPERTY(BlueprintReadWrite)
	UInputMappingContext* DefaultMappingContext;

	
	UPROPERTY()
	UInputAction* InputHorizontalMove;

	FEnhancedInputActionValueBinding* InputHorizontalMoveValueBinding;
	
	UPROPERTY()
	UInputAction* InputJump;
	
	FEnhancedInputActionValueBinding* InputJumpValueBinding;
	
	UPROPERTY()
	UInputAction* InputSpinJump;

	FEnhancedInputActionValueBinding* InputSpinJumpValueBinding;
	
	UPROPERTY()
	UInputAction* InputSpecial;
	
	FEnhancedInputActionValueBinding* InputSpecialValueBinding;
	
	UPROPERTY()
	UInputAction* InputCrouch;
	
	FEnhancedInputActionValueBinding* InputCrouchValueBinding;
	
	UPROPERTY()
	UInputAction* InputLookUp;

	FEnhancedInputActionValueBinding* InputLookupValueBinding;
	
	UPROPERTY()
	UInputAction* InputStart;

	UPROPERTY()
	UInputAction* InputSelect;

	UPROPERTY()
	UInputAction* InputSnapCameraLeft;

	UPROPERTY()
	UInputAction* InputSnapCameraRight;

	UPROPERTY()
	UInputAction* MoveCameraHorizontal;

	UPROPERTY()
	UInputAction* MoveCameraVertical;

	
	UPROPERTY(BlueprintReadWrite)
	UInputMappingContext* MessageBlockMappingContext;

	UPROPERTY()
	UInputAction* InputConfirmMessage;

	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/SuperMarioWorld3D.MarioPhysicsProperties"))
	UDataTable* PhysicsPropertiesTable;
	
	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/SuperMarioWorld3D.MarioJumpVelocities"))
	UDataTable* JumpVelocitiesTable;

	UPROPERTY(EditDefaultsOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/SuperMarioWorld3D.PowerUpData"))
	UDataTable* PowerUpsTable;
	
	UPROPERTY(BlueprintReadWrite)
	float MIN_WALK_SPEED = .17;

	UPROPERTY(BlueprintReadWrite)
	float PHYS_FACTOR = 6;

	UPROPERTY(BlueprintReadWrite)
	float TERMINAL_VELOCITY;

	const float JUMP_HELD_GRAVITY = .1875f;

	const float DEFAULT_GRAVITY = .375f;
private:

	bool DrawDebugTraces;

	
	UPROPERTY()
	ULevelHUD* LevelHUD;

	EMarioStates MarioState;

	FVector2d HorizontalVelocity;

	UPROPERTY()
	TArray<UObject*> fire_balls;

	FMarioPhysicsProperties* PhysicsProperties;

	FMarioJumpVelocities* JumpVelocities;
	
	int32 time_remaining = 300;

	int32 power_state = 0;

	int32 combo_count = 0;
	
	int32 p_meter;
	
	float VerticalVelocity;

	bool holding = false;
	
	bool crouched = false;

	bool looking_up = false;
	
	bool skidding = false;

	bool gravity_enabled = false;

	float jump_buffer_length = .2f;
	
	FTimerHandle skid_timer;
	
	FTimerHandle jump_buffer_timer;

	FTimerHandle spin_jump_buffer_timer;

	FTimerHandle delayed_jump_anim_timer;

	FTimerHandle decrement_time_timer;

	FTimerHandle spin_fire_timer;

	FTimerHandle anim_freeze_timer;

	UPROPERTY()
	ASMWPlayerState* SMW_player_state;
	
	FOnMovementStopped OnMovementStopped;
};
