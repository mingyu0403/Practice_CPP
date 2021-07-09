// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerAnim.h"
#include "CPP_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UCPP_PlayerAnim::UCPP_PlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_SwordAttack0(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/Attack_0.Attack_0'"));
	if (AM_SwordAttack0.Succeeded())
	{
		SwordAttack0 = AM_SwordAttack0.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_SwordAttack1(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/Attack_1.Attack_1'"));
	if (AM_SwordAttack1.Succeeded())
	{
		SwordAttack1 = AM_SwordAttack1.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_SwordAttack2(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/Attack_2.Attack_2'"));
	if (AM_SwordAttack2.Succeeded())
	{
		SwordAttack2 = AM_SwordAttack2.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_AimPose(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/AM_AimPose.AM_AimPose'"));
	if (AM_AimPose.Succeeded())
	{
		AimPose = AM_AimPose.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_GunAttack0(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/AM_GunAttack.AM_GunAttack'"));
	if (AM_GunAttack0.Succeeded())
	{
		GunAttack0 = AM_GunAttack0.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Rolling0(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/AM_Rolling.AM_Rolling'"));
	if (AM_Rolling0.Succeeded())
	{
		Rolling0 = AM_Rolling0.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Dead0(TEXT("AnimMontage'/Game/Blueprints/Player/Anim/AM_Dead.AM_Dead'"));
	if (AM_Dead0.Succeeded())
	{
		Dead0 = AM_Dead0.Object;
	}
}

void UCPP_PlayerAnim::NativeBeginPlay()
{
	Player = Cast<ACPP_Player>(TryGetPawnOwner());
}

void UCPP_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player == false || Player->IsValidLowLevel() == false)
	{
		return;
	}
	
	Speed = Player->GetMovementComponent()->Velocity.Size();

	if (Player->bIsIronSight == true)
	{
		ModelAimPitch = Player->AimPitch;

		if (bUsingIronSight == false)
		{
			bUsingIronSight = true;
			Montage_Play(AimPose, 0.0f);
			ComboIndex = 0;
			bIsSwordMotion = false;
		}
	}
	else
	{
		ModelAimPitch = 0;
		bUsingIronSight = false;
		Montage_Stop(0.3f, AimPose);
	}
}

void UCPP_PlayerAnim::AnimNotify_EndAttack()
{
	ResetMotion();
}

void UCPP_PlayerAnim::AnimNotify_EndCombo()
{
	ComboIndex = 0;
	ResetMotion();
}

void UCPP_PlayerAnim::AnimNotify_EndShot()
{
	bUsingIronSight = false;
	ResetMotion();
}

void UCPP_PlayerAnim::AnimNotify_RollingEnd()
{
	ResetMotion();

	Player->RollingEnd();
	bUsingIronSight = false;
}

void UCPP_PlayerAnim::AnimNotify_GhostTail()
{
	if (Player)
	{
		Player->SpawnGhostTail();
	}
}

void UCPP_PlayerAnim::GunAttack()
{
	if (bIsShotMotion)
	{
		return;
	}
	bIsShotMotion = true;
	Montage_Play(GunAttack0);
}

void UCPP_PlayerAnim::Rolling()
{
	if (bIsRollingMotion)
	{
		return;
	}
	bIsRollingMotion = true;
	Montage_Play(Rolling0);
}

void UCPP_PlayerAnim::ResetMotion()
{
	bIsSwordMotion = false;
	bIsRollingMotion = false;
	bIsShotMotion = false;
}

void UCPP_PlayerAnim::Dead()
{
	Montage_Play(Dead0);
}

void UCPP_PlayerAnim::SwordAttack()
{
	if (bIsSwordMotion)
	{
		return;
	}
	bIsSwordMotion = true;

	switch (ComboIndex)
	{
	case 0:
		Montage_Play(SwordAttack0);
		ComboIndex = 1;
		break;
	case 1:
		Montage_Play(SwordAttack1);
		ComboIndex = 2;
		break;
	case 2:
		Montage_Play(SwordAttack2);
		ComboIndex = 0;
		break;
	default:
		Montage_Play(SwordAttack0);
		ComboIndex = 1;
		break;
	}
}
