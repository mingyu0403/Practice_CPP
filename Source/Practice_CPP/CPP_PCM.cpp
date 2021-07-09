// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PCM.h"
#include "CPP_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACPP_PCM::ACPP_PCM()
{
	// Tick 이벤트를 사용하려면, 따로 체크해줘야 함.
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_PCM::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		APawn* Pawn = PC->GetPawn();
		Player = Cast<ACPP_Player>(Pawn);

		// Player= PC->GetPawn<ACPP_Player>();
	}
}

void ACPP_PCM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Player)
	{
		float TargetFOV = Player->bIsIronSight ? Player->AimFOV : Player->NormalFOV;
		Player->Camera->FieldOfView = 
			FMath::FInterpTo(Player->Camera->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(), 15.0f);
		
		float TargetSpringArmLength = Player->bIsIronSight ? Player->AimSpringArmLength : Player->NormalSpringArmLength;
		Player->SpringArm->TargetArmLength =
			FMath::FInterpTo(Player->SpringArm->TargetArmLength, TargetSpringArmLength, GetWorld()->GetDeltaSeconds(), 15.0f);

		FVector TargetSocketOffset = Player->bIsIronSight ? Player->AimSocketOffset : Player->NormalSocketOffset;
		Player->SpringArm->SocketOffset =
			FMath::VInterpTo(Player->SpringArm->SocketOffset, TargetSocketOffset, GetWorld()->GetDeltaSeconds(), 15.0f);
	}
}
