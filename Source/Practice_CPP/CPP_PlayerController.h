// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACPP_PlayerController();
	virtual void BeginPlay() override;
};
