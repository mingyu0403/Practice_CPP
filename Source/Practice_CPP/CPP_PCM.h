// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CPP_PCM.generated.h"

class ACPP_Player;

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ACPP_PCM : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ACPP_PCM();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	ACPP_Player* Player;
};
