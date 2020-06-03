// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRMenuWidget.generated.h"

class AVRCharacterPawn;

/**
 * 
 */
UCLASS()
class VRBASICS_API UVRMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnDisplayMenuAnimation"))
    void OnDisplayMenuAnimation();
    
    UFUNCTION(BlueprintCallable, BlueprintPure)
    AVRCharacterPawn *GetCharacterPawn();
};
