
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

UCLASS()
class PRACTICE_CPP_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NoticeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayerCountText;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatMessageInputText;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameBtn;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatArea;

	void SetNoticeText(FText Text);
	void SetCurrentPlayerCountText(FText Text);

	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	void AddChatMessage(FText Text);

	UFUNCTION()
	void OnStartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnim();
};
