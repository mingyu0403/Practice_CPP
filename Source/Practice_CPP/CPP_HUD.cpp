// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/CWidget_HPProgress.h"

ACPP_HUD::ACPP_HUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> UW_CrossHair(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WG_CrossHair.WG_CrossHair_C'"));
	if(UW_CrossHair.Succeeded())
	{
		CrossHair = CreateWidget<UUserWidget>(GetWorld(), UW_CrossHair.Class);
		if (CrossHair)
		{
			CrossHair->AddToViewport();
			ShowCrossHair(false);
		}
	}
	ConstructorHelpers::FClassFinder<UCWidget_HPProgress> UW_HP_Progress(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WG_PlayerHP.WG_PlayerHP_C'"));
	if (UW_HP_Progress.Succeeded())
	{
		HP_Progress = CreateWidget<UCWidget_HPProgress>(GetWorld(), UW_HP_Progress.Class);
		if (HP_Progress)
		{
			HP_Progress->AddToViewport();
		}
	}
}

void ACPP_HUD::BeginPlay()
{
	
}

void ACPP_HUD::ShowCrossHair(bool bIsShow)
{
	if (CrossHair)
	{
		CrossHair->SetVisibility(bIsShow == true ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ACPP_HUD::SetHP(float MaxHP, float CurrentHP)
{
	if (HP_Progress)
	{
		HP_Progress->SetHPProgress(CurrentHP / MaxHP);
		HP_Progress->SetHPLabel(FText::FromString(FString::Printf(TEXT("%.1f/%.1f"), CurrentHP, MaxHP)));
		//HP_Progress->SetHPLabel(FText::Format(FText::FromString(TEXT("{0}/{1}")), CurrentHP, MaxHP));
	}
}
