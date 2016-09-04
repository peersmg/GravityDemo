// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGravityDemo.h"
#include "GravCharacter.h"


// Sets default values
AGravCharacter::AGravCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  CurrentCapsuleRotation = GetCapsuleComponent()->RelativeRotation;
  /*
  OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
  OurCameraSpringArm->AttachTo(RootComponent);
  OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
  OurCameraSpringArm->TargetArmLength = 400.f;
  OurCameraSpringArm->bEnableCameraLag = true;
  OurCameraSpringArm->CameraLagSpeed = 3.0f;

  OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
  OurCamera->AttachTo((USceneComponent*)OurCameraSpringArm, USpringArmComponent::SocketName);
  */
}

// Called when the game starts or when spawned
void AGravCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

  // Apply artificial gravity
  FVector toCentre = currentPlanet->GetActorLocation() - GetActorLocation();
  toCentre.Normalize();
  GetMovementComponent()->Velocity += toCentre*gravityScale*DeltaTime;

  //Apply artificial rotation
  UpdateCapsuleRotation(DeltaTime, -toCentre, true, 10.0f);
}

// Called to bind functionality to input
void AGravCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

  InputComponent->BindAxis("MoveForward", this, &AGravCharacter::MoveForward);
  InputComponent->BindAxis("MoveRight", this, &AGravCharacter::MoveRight);

  InputComponent->BindAxis("MouseX", this, &AGravCharacter::AddControllerYawInput);
  InputComponent->BindAxis("MouseY", this, &AGravCharacter::AddControllerPitchInput);

  InputComponent->BindAction("Jump", IE_Pressed, this, &AGravCharacter::OnStartJump);
  InputComponent->BindAction("Jump", IE_Released, this, &AGravCharacter::OnStopJump);

}

void AGravCharacter::MoveForward(float Val)
{
  if ((Controller != NULL) && (Val != 0.0f))
  {
    // find out which way is forward
    FRotator Rotation = GetController()->GetControlRotation();
    
    // add movement in that direction
    const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

    AddMovementInput(Direction, Val);
  }
}

void AGravCharacter::MoveRight(float Val)
{
  if ((Controller != NULL) && (Val != 0.0f))
  {
    // find out which way is right
    const FRotator Rotation = GetController()->GetControlRotation();
    const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
    // add movement in that direction
    AddMovementInput(Direction, Val);
  }
}

void AGravCharacter::OnStartJump()
{
  bPressedJump = true;
}

void AGravCharacter::OnStopJump()
{
  bPressedJump = false;
}

void AGravCharacter::UpdateCapsuleRotation(float DeltaTime, const FVector& TargetUpVector, bool bInstantRot, float RotationSpeed)
{
  const FVector CapsuleUp = GetCapsuleComponent()->GetUpVector();
  const FQuat DeltaQuat = FQuat::FindBetween(CapsuleUp, TargetUpVector);
  const FQuat TargetQuat = DeltaQuat * GetCapsuleComponent()->GetComponentRotation().Quaternion();

  CurrentCapsuleRotation = bInstantRot ? TargetQuat.Rotator() : FMath::RInterpTo(CurrentCapsuleRotation, TargetQuat.Rotator(), DeltaTime, RotationSpeed);

  GetCapsuleComponent()->SetWorldRotation(CurrentCapsuleRotation);
  Controller->SetControlRotation(FRotator(GetControlRotation().Pitch + DeltaQuat.Rotator().Pitch, GetControlRotation().Yaw + DeltaQuat.Rotator().Yaw, GetControlRotation().Roll + DeltaQuat.Rotator().Roll));
}

