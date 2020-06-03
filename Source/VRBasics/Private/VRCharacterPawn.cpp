// Fill out your copyright notice in the Description page of Project Settings.

//This Include
#include "VRCharacterPawn.h"
// Local Includes
#include "VRMenuWidget.h"
// UE4 Includes
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/WidgetComponent.h"

//MARK: - Constants -

namespace AVRCharacterPawnConstants
{
    static const FName OculusSystemName(TEXT("OculusHMD"));
}

//MARK: - Constructor -

AVRCharacterPawn::AVRCharacterPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneComponent;

    VROriginSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
    VROriginSceneComponent->SetupAttachment(DefaultSceneComponent);

    VRCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCameraComponent"));
    VRCameraComponent->SetupAttachment(VROriginSceneComponent);

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetupAttachment(VROriginSceneComponent);

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(VROriginSceneComponent);
}

//MARK: - Game lifeCycle -

void AVRCharacterPawn::BeginPlay()
{
    Super::BeginPlay();

    WidgetComponent->SetVisibility(false);
    SetupVREnvironment();
    SetupVRHands();
}

void AVRCharacterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AVRCharacterPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//MARK: - BlueprintCallable Methods -

void AVRCharacterPawn::MovePawnRight(float AxisValue)
{
    FVector RightCameraVector = FVector::VectorPlaneProject(VRCameraComponent->GetRightVector(), FVector(0.0f, 0.0f, 1.0f));
    MovePawnFromVector(RightCameraVector, AxisValue);
}

void AVRCharacterPawn::MovePawnForward(float AxisValue)
{
    FVector ForwardCameraVector = FVector::VectorPlaneProject(VRCameraComponent->GetForwardVector(), FVector(0.0f, 0.0f, 1.0f));
    MovePawnFromVector(ForwardCameraVector, AxisValue);
}

void AVRCharacterPawn::DisplayWidget(bool bShowWidget)
{
    if (UUserWidget *UserWidget = WidgetComponent->GetUserWidgetObject())
    {
        if (UVRMenuWidget *MenuWidget = Cast<UVRMenuWidget>(UserWidget))
        {
            WidgetComponent->SetVisibility(bShowWidget);

            if (bShowWidget)
            {
                MenuWidget->OnDisplayMenuAnimation();
            }

            if (RightHandActor)
            {
                RightHandActor->GetWidgetInteractionComponent()->bShowDebug = bShowWidget;
            }
        }
    }
}

void AVRCharacterPawn::SetMovement(EVRMovementType Type)
{
    MovementType = Type;
}

//MARK: - Private Methods -

void AVRCharacterPawn::SetupVREnvironment()
{
    FName DeviceType = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

    if (DeviceType == AVRCharacterPawnConstants::OculusSystemName)
    {
        UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
    }
}

void AVRCharacterPawn::SetupVRHands()
{
    if (ensureMsgf(MotionControllerStaticClass, TEXT("Please select a AVRMotionControllerActor Blueprint")))
    {
        LeftHandActor = SpawnMotionControllerActor(EMotionHandController::Left);
        RightHandActor = SpawnMotionControllerActor(EMotionHandController::Right);
    }
}

AVRMotionControllerActor *AVRCharacterPawn::SpawnMotionControllerActor(EMotionHandController MotionHandControllerType)
{
    FActorSpawnParameters ActorSpawnParameters;
    ActorSpawnParameters.Owner = this;

    AVRMotionControllerActor *MotionControllerActor = GetWorld()->SpawnActor<AVRMotionControllerActor>(MotionControllerStaticClass, FTransform::Identity, ActorSpawnParameters);

    if (ensureMsgf(MotionControllerActor, TEXT("MotionControllerActor should not be null")))
    {
        MotionControllerActor->SetMotionHandControllerType(MotionHandControllerType, this);
        MotionControllerActor->AttachToComponent(VROriginSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
    }

    return MotionControllerActor;
}

void AVRCharacterPawn::MovePawnFromVector(FVector Direction, float AxisValue)
{
    if (MovementType == EVRMovementType::Joystick)
    {
        AddActorWorldOffset(Direction * AxisValue * SpeedMovement * GetWorld()->GetDeltaSeconds());
    }
}
