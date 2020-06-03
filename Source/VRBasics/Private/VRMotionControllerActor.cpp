// Fill out your copyright notice in the Description page of Project Settings.

//This Include
#include "VRMotionControllerActor.h"
// Local Includes
#include "VRCharacterPawn.h"
// UE4 Includes
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "NavigationSystem.h"

#if PLATFORM_ANDROID
#include "OculusFunctionLibrary.h"
#endif

//MARK: - Constants -

namespace AVRMotionControllerConstants
{
    static const FName MotionSourceRightName(TEXT("Right"));
    static const FName MotionSourceLeftName(TEXT("Left"));
} // namespace AVRMotionControllerConstants

//MARK: - Constructor -

AVRMotionControllerActor::AVRMotionControllerActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneComponent;

    MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
    MotionControllerComponentRight->MotionSource = AVRMotionControllerConstants::MotionSourceRightName;
    MotionControllerComponentRight->SetupAttachment(DefaultSceneComponent);

    TeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportMesh"));
    TeleportMesh->SetupAttachment(DefaultSceneComponent);

    SkeletalHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalHand"));
    SkeletalHand->SetupAttachment(MotionControllerComponentRight);

    GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
    GrabSphere->SetupAttachment(SkeletalHand);

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(SkeletalHand);

    WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
    WidgetInteractionComponent->SetupAttachment(SkeletalHand);

    MotionHandLeftScale = FVector(1.0f, 1.0f, -1.0f);
}

//MARK: - Game lifeCycle -

void AVRMotionControllerActor::BeginPlay()
{
    Super::BeginPlay();
    Init();
}

void AVRMotionControllerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TeleportDestination();
}

//MARK: - Public Methods -
//MARK: - BlueprintCallable Methods -

void AVRMotionControllerActor::SetMotionHandControllerType(EMotionHandController MotionHandControllerType, AVRCharacterPawn *CharacterPawn)
{
    MotionControllerComponentRight->MotionSource = GetHandNameByType(MotionHandControllerType);

    MotionHand = MotionHandControllerType;
    ParentPawn = CharacterPawn;

    if (MotionHandControllerType == EMotionHandController::Left)
    {
        SkeletalHand->SetWorldScale3D(MotionHandLeftScale);
    }

    OnMontionControllerFinishToSetup();
}

void AVRMotionControllerActor::TeleportDestination()
{
    FVector StartVector = MotionControllerComponentRight->GetComponentLocation();
    FVector ForwardVector = MotionControllerComponentRight->GetForwardVector();

    FPredictProjectilePathParams Params(ProjectileRadius,
                                        StartVector,
                                        ForwardVector * TeleportLaunchVelocity,
                                        MaxSimTime,
                                        ECollisionChannel::ECC_Visibility,
                                        this);

    Params.bTraceComplex = true;
    Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;

    FPredictProjectilePathResult Result;
    bool bHasHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

    if (!bHasHit)
        return;

    TArray<FVector> PathLocationArray;
    for (FPredictProjectilePathPointData PredictProjectilePathPointData : Result.PathData)
    {
        PathLocationArray.Add(PredictProjectilePathPointData.Location);
    }

    FillSplinePoints(PathLocationArray);

    FNavLocation OutLocation;
    FVector ExtentVector = FVector(ProjectNavExtends);
    bool bHasHitNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(Result.HitResult.Location, OutLocation, ExtentVector);
    bool bCanMoveTeleportMesh = bHasHitNavMesh && bHasHit;

    TeleportMesh->SetVisibility(bCanMoveTeleportMesh);

    if (bCanMoveTeleportMesh)
    {
        TeleportMesh->SetWorldLocation(OutLocation.Location);
    }
}

void AVRMotionControllerActor::StartTeleportDestination()
{
    if (ParentPawn && ParentPawn->GetMovementType() == EVRMovementType::Teleport)
    {
        SetActorTickEnabled(true);
    }
}

void AVRMotionControllerActor::FinishTeleportDestination()
{
    if (ParentPawn && ParentPawn->GetMovementType() == EVRMovementType::Teleport)
    {
        SetActorTickEnabled(false);
        TeleportMesh->SetVisibility(false);
        EnableFaceOutBeforeTeleport();
    }
}

void AVRMotionControllerActor::SetGripType(EGripType Grip)
{
    GripType = Grip;
}

//MARK: - Private Methods -

void AVRMotionControllerActor::Init()
{
    TeleportMesh->SetVisibility(false);
}

FName AVRMotionControllerActor::GetHandNameByType(EMotionHandController MotionHandControllerType)
{
    if (MotionHandControllerType == EMotionHandController::Right)
    {
        return AVRMotionControllerConstants::MotionSourceRightName;
    }
    else
    {
        return AVRMotionControllerConstants::MotionSourceLeftName;
    }
}

void AVRMotionControllerActor::EnableFaceOutBeforeTeleport()
{
    bool bIsFadeOutTimerActive = GetWorldTimerManager().IsTimerActive(FadeInHandler);

    if (!bIsFadeOutTimerActive)
    {
        HandleFadeCamera(FLinearColor(0, 0, 0, 0), FLinearColor(1, 1, 1, 1));
        GetWorldTimerManager().SetTimer(FadeInHandler, this, &AVRMotionControllerActor::EnableFaceInAfterTeleport, TeleportFadeInDelayTime, false);
    }
}

void AVRMotionControllerActor::EnableFaceInAfterTeleport()
{
    if (ParentPawn)
    {
        FVector TeleportPosition = TeleportMesh->GetComponentLocation();
        TeleportPosition += ParentPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * ParentPawn->GetActorUpVector();
        ParentPawn->SetActorLocation(TeleportPosition);
        HandleFadeCamera(FLinearColor(1, 1, 1, 1), FLinearColor(0, 0, 0, 0));
    }
}

void AVRMotionControllerActor::FillSplinePoints(TArray<FVector> PathLocationArray)
{
    SplineComponent->ClearSplinePoints(false);
    for (int index = 0; index < PathLocationArray.Num(); ++index)
    {
        FVector RelativePosition = SplineComponent->GetComponentTransform().InverseTransformPosition(PathLocationArray[index]);
        FSplinePoint SplinePoint(index, RelativePosition, ESplinePointType::Curve);
        SplineComponent->AddPoint(SplinePoint, false);
    }

    SplineComponent->UpdateSpline();
}

void AVRMotionControllerActor::HandleFadeCamera(FLinearColor FromAlpha, FLinearColor ToAlpha)
{
#if PLATFORM_ANDROID
    UOculusFunctionLibrary::SetColorScaleAndOffset(FromAlpha, ToAlpha, true);
#endif
}
