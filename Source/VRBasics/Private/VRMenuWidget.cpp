// Fill out your copyright notice in the Description page of Project Settings.

//This Include
#include "VRMenuWidget.h"
// Local Includes
#include "VRCharacterPawn.h"
// UE4 Includes
#include "Kismet/GameplayStatics.h"

AVRCharacterPawn *UVRMenuWidget::GetCharacterPawn()
{
    AActor *ActorFound = UGameplayStatics::GetActorOfClass(GetWorld(), AVRCharacterPawn::StaticClass());
    if (ActorFound)
    {
        return Cast<AVRCharacterPawn>(ActorFound);
    }

    return nullptr;
}
