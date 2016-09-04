// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGravityDemo.h"
#include "GravPlayer.h"


// Sets default values
AGravPlayer::AGravPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  SetRootComponent(CapsuleComponent);

  OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
  OurCamera->AttachTo(RootComponent);

  DirArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
  DirArrow->AttachTo(RootComponent);

  AutoPossessPlayer = EAutoReceiveInput::Player0;
  AutoReceiveInput = EAutoReceiveInput::Player0;

  CapsuleComponent->SetSimulatePhysics(true);
  CapsuleComponent->SetCapsuleHalfHeight(88.0f);
  CapsuleComponent->SetCapsuleRadius(34.0f);
  CapsuleComponent->SetCollisionProfileName(FName("Pawn"));
  CapsuleComponent->UpdateCollisionProfile();
  

  OurCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

  CurrentCapsuleRotation = CapsuleComponent->RelativeRotation;

  gravityStrength = 960.0f;
  walkSpeed = 320.0f;
  jumpForce = 10.0f;
  //For testing
  gravityDirection = FVector(0,0,-1);
}


// Called when the game starts or when spawned
void AGravPlayer::BeginPlay()
{
	Super::BeginPlay();

  if (pPhysicsMat != nullptr)
  {
    CapsuleComponent->SetPhysMaterialOverride(pPhysicsMat);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Player physics material not set at begin!"));
  }

  if (pGravityPoint == nullptr)
  {
    UE_LOG(LogTemp, Fatal, TEXT("Gravity point not set at begin!"));
  }
}


// Called every frame
void AGravPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

  FRotator localRotation =  OurCamera->RelativeRotation;
  
  // Apply artificial gravity
  if (pGravityPoint != nullptr)
  {
    gravityDirection = pGravityPoint->GetActorLocation() - GetActorLocation();
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Gravity point not set at runtime!"));
  }
  
  gravityDirection.Normalize();
  CapsuleComponent->AddForce(gravityDirection * gravityStrength * CapsuleComponent->GetMass());

  // Apply artificial rotation
  UpdateCapsuleRotation(DeltaTime, -gravityDirection, true, 10.0f);

  // Prevent the capsule from rotating due to gravity
  CapsuleComponent->SetPhysicsAngularVelocity(FVector(0, 0, 0));

  // 1. Linetrace down from centre and four edges to check if grounded or not
  // 2. Set grounded variable
}


// Called to bind functionality to input
void AGravPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

  InputComponent->BindAxis("MoveForward", this, &AGravPlayer::MoveForward);
  InputComponent->BindAxis("MoveRight", this, &AGravPlayer::MoveRight);

  InputComponent->BindAxis("MouseX", this, &AGravPlayer::MouseXMove);
  InputComponent->BindAxis("MouseY", this, &AGravPlayer::MouseYMove);

  InputComponent->BindAction("Jump", IE_Pressed, this, &AGravPlayer::OnStartJump);
  //InputComponent->BindAction("Jump", IE_Released, this, &AGravPlayer::OnStopJump);
}


void AGravPlayer::MoveForward(float val)
{ // Move the player foreward

  // 1. Linetrace foreward and back from centre of capsule and both edges
  // 2. If hit anything dont move foreward
  // 3. Expose length of line trace which will determine the steepness of the slope the character can walk up

  FVector velocity = CapsuleComponent->GetForwardVector() * walkSpeed * val;
  SetActorLocation(GetActorLocation() + velocity*GWorld->DeltaTimeSeconds);
}


void AGravPlayer::MoveRight(float val)
{ // Move the player left or right

  // 1. Linetrace left and right from centre of capsule and both edges
  // 2. If hit anything dont move foreward
  // 3. Expose length of line trace which will determine the steepness of the slope the character can walk up

  FVector velocity = CapsuleComponent->GetRightVector() * walkSpeed * val;
  SetActorLocation(GetActorLocation() + velocity*GWorld->DeltaTimeSeconds);
}


void AGravPlayer::OnStartJump()
{ // Let the player jump

  CapsuleComponent->AddForce(-gravityDirection * gravityStrength * jumpForce * CapsuleComponent->GetMass());
}


void AGravPlayer::MouseXMove(float val)
{ // Mouse X movement

  CapsuleComponent->AddLocalRotation(FRotator(0, val, 0)); 
}


void AGravPlayer::MouseYMove(float val)
{ // Mouse Y movement

  if (OurCamera->RelativeRotation.Pitch + (-val) >= 85.0f)
  {
    OurCamera->RelativeRotation.Pitch = 85.0f;
  }
  else if (OurCamera->RelativeRotation.Pitch + (-val) <= -85.0f)
  {
    OurCamera->RelativeRotation.Pitch = -85.0f;
  }
  else
  {
    OurCamera->AddLocalRotation(FRotator(-val, 0, 0));
  }
}


void AGravPlayer::UpdateCapsuleRotation(float DeltaTime, const FVector& TargetUpVector, bool bInstantRot, float RotationSpeed)
{ // Sets the rotation of the capsule based on position in relation to planet

  const FVector CapsuleUp = CapsuleComponent->GetUpVector();
  const FQuat DeltaQuat = FQuat::FindBetween(CapsuleUp, TargetUpVector);
  const FQuat TargetQuat = DeltaQuat * CapsuleComponent->GetComponentRotation().Quaternion();

  CurrentCapsuleRotation = TargetQuat.Rotator();
  CapsuleComponent->SetWorldRotation(CurrentCapsuleRotation);
}
