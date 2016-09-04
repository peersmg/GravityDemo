// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GravPlayer.generated.h"

UCLASS()
class SPACEGRAVITYDEMO_API AGravPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGravPlayer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  //handles moving forward/backward
  UFUNCTION()
    void MoveForward(float Val);
  //handles strafing
  UFUNCTION()
    void MoveRight(float Val);

  //sets jump flag when key is pressed
  UFUNCTION()
    void OnStartJump();

  UFUNCTION()
    void MouseXMove(float val);

  UFUNCTION()
    void MouseYMove(float val);

  UFUNCTION()
    void UpdateCapsuleRotation(float DeltaTime, const FVector& TargetUpVector, bool bInstantRot, float RotationSpeed);

  UCapsuleComponent* CapsuleComponent;
  UCameraComponent* OurCamera;
  UArrowComponent* DirArrow;

  UPROPERTY(EditAnywhere)
    UPhysicalMaterial* pPhysicsMat;

  UPROPERTY(EditAnywhere)
    float gravityStrength;

  UPROPERTY(EditAnywhere)
    float walkSpeed;

  UPROPERTY(EditAnywhere)
    float jumpForce;

  UPROPERTY(EditAnywhere)
    AActor* pGravityPoint;          // Pointer to the current planet

  FVector gravityDirection;         // The current direction of gravity
  bool grounded;                    // Is the player touching the ground

  FRotator CurrentCapsuleRotation;  // The current rotation of the capsule
  
};
