// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPP_HUD.generated.h"

class UUserWidget;
class UCWidget_HPProgress;

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ACPP_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ACPP_HUD();
	
public:
	virtual void BeginPlay() override;
	
public:
	void ShowCrossHair(bool bIsShow);
	void SetHP(float MaxHP, float CurrentHP);

public:
	UUserWidget* CrossHair;
	UCWidget_HPProgress* HP_Progress;
};
