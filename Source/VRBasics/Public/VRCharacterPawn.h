// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE4 Includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
// Local Includes
#include "VRMotionControllerActor.h"
// Generated Include
#include "VRCharacterPawn.generated.h"

class AVRMotionControllerActor;
class UCameraComponent;
class UCapsuleComponent;
class USceneComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EVRMovementType : uint8
{
    Joystick,
    Teleport
};

/** Class for handle VR Character for OculusQuest. */
UCLASS()
class VRBASICS_API AVRCharacterPawn : public APawn
{
    GENERATED_BODY()

public:
    AVRCharacterPawn();

    /** Returns Camera Component  **/
    FORCEINLINE UCameraComponent *GetCameraComponent() const { return VRCameraComponent; }
    EVRMovementType GetMovementType() const { return MovementType; }

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = VRCharacterPawn)
    void MovePawnRight(float AxisValue);

    UFUNCTION(BlueprintCallable, Category = VRCharacterPawn)
    void MovePawnForward(float AxisValue);

    UFUNCTION(BlueprintCallable)
    void DisplayWidget(bool bShowWidget);

    UFUNCTION(BlueprintCallable)
    void SetMovement(EVRMovementType Type);

    /** Class reference of AMotionControllerActor to be spawn and attach on AVRCharacterPawn. */
    UPROPERTY(EditDefaultsOnly, Category = VRCharacterPawn)
    TSubclassOf<AVRMotionControllerActor> MotionControllerStaticClass;

    /** Reference of the created left MotionControllerActor. */
    UPROPERTY(BlueprintReadOnly)
    AVRMotionControllerActor *LeftHandActor;

    /** Reference of the created right MotionControllerActor. */
    UPROPERTY(BlueprintReadOnly)
    AVRMotionControllerActor *RightHandActor;

    /** Speed value of the locomotion movement. */
    UPROPERTY(EditAnywhere)
    float SpeedMovement;

protected:
    virtual void BeginPlay() override;

private:
    void SetupVREnvironment();
    void SetupVRHands();
    void MovePawnFromVector(FVector Direction, float AxisValue);

    AVRMotionControllerActor *SpawnMotionControllerActor(EMotionHandController MotionHandControllerType);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VRCharacterPawn, meta = (AllowPrivateAccess = "true"))
    USceneComponent *VROriginSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VRCharacterPawn, meta = (AllowPrivateAccess = "true"))
    USceneComponent *DefaultSceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VRCharacterPawn, meta = (AllowPrivateAccess = "true"))
    UCameraComponent *VRCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VRCharacterPawn, meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent *CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VRCharacterPawn, meta = (AllowPrivateAccess = "true"))
    UWidgetComponent *WidgetComponent;

    UPROPERTY()
    EVRMovementType MovementType;
};
