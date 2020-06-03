// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// Local Includes
#include "VRMotionControllerActor.h"
// UE4 Includes
#include "Animation/AnimInstance.h"
// Generated Include
#include "VRHandAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class VRBASICS_API UVRHandAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UVRHandAnimInstance();

    virtual void NativeUpdateAnimation(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UVRHandAnimInstance)
    float InterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UVRHandAnimInstance)
    float Grip;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UVRHandAnimInstance)
    EGripType GripState;
};
