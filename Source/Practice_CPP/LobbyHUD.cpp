// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"
#include "LobbyUI.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateLobbyWidget();
}

void ALobbyHUD::CreateLobbyWidget()
{
	if (LobbyUIObject == nullptr)
	{
		if (LobbyUIClass)
		{
			LobbyUIObject = CreateWidget<ULobbyUI>(GetOwningPlayerController(), LobbyUIClass);
			if (LobbyUIObject)
			{
				LobbyUIObject->AddToViewport();
			}
		}
	}
}

void ALobbyHUD::SetNoticeMessage(int32 CountTime)
{
	if (LobbyUIObject == nullptr)
	{
		CreateLobbyWidget();
	}
	LobbyUIObject->SetNoticeText(FText::Format(FText::FromString(TEXT("{0}초 남았습니다.")), CountTime));
}

void ALobbyHUD::SetCurrentPlayerCountsMessage(int32 Counts)
{
	if (LobbyUIObject == nullptr)
	{
		CreateLobbyWidget();
	}
	LobbyUIObject->SetCurrentPlayerCountText(FText::AsNumber(Counts));
}

void ALobbyHUD::AddChatMessage(FText Text)
{
	if (LobbyUIObject == nullptr)
	{
		CreateLobbyWidget();
	}
	LobbyUIObject->AddChatMessage(Text);
}

void ALobbyHUD::ShowStartAnim()
{
	if (LobbyUIObject == nullptr)
	{
		CreateLobbyWidget();
	}
	LobbyUIObject->OnPlayAnim();
}
