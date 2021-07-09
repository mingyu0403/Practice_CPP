// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_BasicHPProgress.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API UCWidget_BasicHPProgress : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* HPProgress;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* HPLabel;

public:
	void SetHPProgress(float Percent);
	void SetHPLabel(FText Text);
};
