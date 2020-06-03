// Fill out your copyright notice in the Description page of Project Settings.

//This Include
#include "VRHandAnimInstance.h"

UVRHandAnimInstance::UVRHandAnimInstance()
{
    InterpSpeed = 7.0f;
}

void UVRHandAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (AActor *Actor = GetOwningActor())
    {
        if (AVRMotionControllerActor *VRMotionControllerActor = Cast<AVRMotionControllerActor>(Actor))
        {
            GripState = VRMotionControllerActor->GripType;
            int32 GripIndexValue = (int32)GripState;
            float GripFloatValue = (float)GripIndexValue;
            float ResultValue = GripFloatValue / 2.0f;
            float InterpValue = FMath::FInterpConstantTo(Grip, ResultValue, DeltaTime, InterpSpeed);
            Grip = InterpValue;
        }
    }
}
