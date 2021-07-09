// Fill out your copyright notice in the Description page of Project Settings.


#include "CWidget_HPProgress.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCWidget_HPProgress::SetHPProgress(float Percent)
{
	HPProgress->SetPercent(Percent);
}

void UCWidget_HPProgress::SetHPLabel(FText Text)
{
	HPLabel->SetText(Text);
}
