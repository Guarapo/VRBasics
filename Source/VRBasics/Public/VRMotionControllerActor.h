// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE4 Includes
#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
// Generated Include
#include "VRMotionControllerActor.generated.h"

class AVRCharacterPawn;
class UMotionControllerComponent;
class USceneComponent;
class USkeletalMeshComponent;
class USplineComponent;
class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EMotionHandController : uint8
{
    Left,
    Right
};

UENUM(BlueprintType)
enum class EGripType : uint8
{
    Open,
    CanGrab,
    Grab
};

UCLASS()
class VRBASICS_API AVRMotionControllerActor : public AActor
{
    GENERATED_BODY()

public:
    AVRMotionControllerActor();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UWidgetInteractionComponent *GetWidgetInteractionComponent() const { return WidgetInteractionComponent; }

    UFUNCTION(BlueprintCallable)
    void SetMotionHandControllerType(EMotionHandController MotionHandControllerType, AVRCharacterPawn *CharacterPawn);

    UFUNCTION(BlueprintCallable)
    void TeleportDestination();

    UFUNCTION(BlueprintCallable)
    void StartTeleportDestination();

    UFUNCTION(BlueprintCallable)
    void FinishTeleportDestination();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Animation")
    EGripType GripType;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VRMotionControllerActor)
    EMotionHandController MotionHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VRMotionControllerActor)
    FVector MotionHandLeftScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Teleport | Projectile")
    float ProjectileRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Teleport | Projectile")
    float TeleportLaunchVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Teleport | Projectile")
    float MaxSimTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Teleport")
    float TeleportFadeInDelayTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRMotionControllerActor | Teleport")
    float ProjectNavExtends;

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnMontionControllerFinishToSetup();

private:
    UFUNCTION()
    void Init();

    UFUNCTION()
    void EnableFaceOutBeforeTeleport();

    UFUNCTION()
    void EnableFaceInAfterTeleport();

    UFUNCTION()
    void FillSplinePoints(TArray<FVector> PathLocationArray);
    
    UFUNCTION()
    void HandleFadeCamera(FLinearColor FromAlpha, FLinearColor ToAlpha);

    UFUNCTION()
    FName GetHandNameByType(EMotionHandController MotionHandControllerType);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent *DefaultSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UMotionControllerComponent *MotionControllerComponentRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *SkeletalHand;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USphereComponent *GrabSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *TeleportMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USplineComponent *SplineComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UWidgetInteractionComponent *WidgetInteractionComponent;

    UPROPERTY()
    AVRCharacterPawn *ParentPawn;

    UPROPERTY()
    FTimerHandle FadeInHandler;
};
