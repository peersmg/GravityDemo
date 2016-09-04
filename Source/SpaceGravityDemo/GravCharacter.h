// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "GravCharacter.generated.h"

UCLASS()
class SPACEGRAVITYDEMO_API AGravCharacter : public ACharacter
{
	GENERATED_BODY()

    /*
protected:
  UPROPERTY(EditAnywhere)
    USpringArmComponent* OurCameraSpringArm;
  UPROPERTY(EditAnywhere)
  UCameraComponent* OurCamera;
  */
public:
	// Sets default values for this character's properties
	AGravCharacter();

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
  //clears jump flag when key is released
  UFUNCTION()
    void OnStopJump();

  // Update the rotation
  UFUNCTION()
    void UpdateCapsuleRotation(float DeltaTime, const FVector& TargetUpVector, bool bInstantRot, float RotationSpeed);

  UPROPERTY(EditAnywhere)
    AActor* currentPlanet;

  UPROPERTY(EditAnywhere)
    float gravityScale;

  FRotator CurrentCapsuleRotation;
	
};
