#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "TitileUI.generated.h"

UCLASS()
class PRACTICE_CPP_API UTitileUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* UserNameText;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PasswordText;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressText;

	FHttpModule* Http;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartServerBtn;
	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectServerBtn;
	UPROPERTY(meta = (BindWidget))
	class UButton* CheckIDBtn;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartServerHandler();
	UFUNCTION()
	void ConnectServerHandler();
	UFUNCTION()
	void CheckIDHandler();

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool wasSuccess);
};
