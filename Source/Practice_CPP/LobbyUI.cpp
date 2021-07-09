// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGM.h"
#include "LobbyPC.h"

void ULobbyUI::NativeConstruct()
{
	ChatMessageInputText->OnTextCommitted.AddDynamic(this, &ULobbyUI::OnChatTextCommitted);
	StartGameBtn->OnClicked.AddDynamic(this, &ULobbyUI::OnStartGame);
}

void ULobbyUI::SetNoticeText(FText Text)
{
	NoticeText->SetText(Text);
}

void ULobbyUI::SetCurrentPlayerCountText(FText Text)
{
	CurrentPlayerCountText->SetText(Text);
}

void ULobbyUI::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		 ALobbyPC* _PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		 if (_PC == nullptr) return;

		 _PC->SendMessage(Text);

		 ChatMessageInputText->SetText(FText::GetEmpty());
	}
}

void ULobbyUI::AddChatMessage(FText Text)
{
	UTextBlock* TextBlock = NewObject<UTextBlock>(ChatArea);
	TextBlock->SetText(Text);

	ChatArea->AddChild(TextBlock);
	ChatArea->ScrollToEnd();
}

void ULobbyUI::OnStartGame()
{
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}