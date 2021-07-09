// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BearAnim.h"
#include "CPP_Bear.h"
#include "GameFramework/PawnMovementComponent.h"

void UCPP_BearAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Bear = Cast<ACPP_Bear>(TryGetPawnOwner());
}

void UCPP_BearAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Bear == nullptr || Bear->IsValidLowLevel() == false)
	{
		return;
	}
	Speed = Bear->GetMovementComponent()->Velocity.Size();
}

void UCPP_BearAnim::AnimNotify_AttackRelease()
{
	SetIsAttackTrigger(false);
}

void UCPP_BearAnim::SetIsAttackTrigger(bool isOn)
{
	bIsAttackTrigger = isOn;
}
