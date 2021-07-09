// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GM.h"
#include "CPP_Player.h"
#include "CPP_PlayerController.h"
#include "CPP_HUD.h"
#include "UObject/ConstructorHelpers.h"

ACPP_GM::ACPP_GM()
{
	ConstructorHelpers::FClassFinder<ACPP_Player> BP_Player(TEXT("Blueprint'/Game/Blueprints/Player/BP_Player.BP_Player_C'"));
	if (BP_Player.Succeeded())
	{
		DefaultPawnClass = BP_Player.Class;
	}
	PlayerControllerClass = ACPP_PlayerController::StaticClass();
	HUDClass = ACPP_HUD::StaticClass();
}