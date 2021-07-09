// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_PlayerController.h"
#include "CPP_PCM.h"

ACPP_PlayerController::ACPP_PlayerController()
{
	PlayerCameraManagerClass = ACPP_PCM::StaticClass();
}

void ACPP_PlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
}
