// Fill out your copyright notice in the Description page of Project Settings.


#include "TitileUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "TitlePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_GameInstance.h"
#include "TitileUI.h"

void UTitileUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartServerBtn->OnClicked.AddDynamic(this, &UTitileUI::StartServerHandler);
	ConnectServerBtn->OnClicked.AddDynamic(this, &UTitileUI::ConnectServerHandler);
	CheckIDBtn->OnClicked.AddDynamic(this, &UTitileUI::CheckIDHandler);

	Http = &FHttpModule::Get();
}

void UTitileUI::StartServerHandler()
{
	UCPP_GameInstance* MyGameInstance = Cast<UCPP_GameInstance>(GetGameInstance());
	if (nullptr == MyGameInstance) return;

	MyGameInstance->SetUserName(UserNameText->GetText().ToString());

	ATitlePlayerController* PC = Cast<ATitlePlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->StartServer();
	}
}

void UTitileUI::ConnectServerHandler()
{
	UCPP_GameInstance* MyGameInstance = Cast<UCPP_GameInstance>(GetGameInstance());
	if (nullptr == MyGameInstance) return;

	MyGameInstance->SetUserName(UserNameText->GetText().ToString());

	ATitlePlayerController* PC = Cast<ATitlePlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->ConnectServer(IPAddressText->GetText().ToString());
	}
}

void UTitileUI::CheckIDHandler()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UTitileUI::OnResponseReceived);
	FString URL = FString::Printf(TEXT("http://192.168.0.200:8888/?id=%s&pwd=%s"), *(UserNameText->GetText().ToString()), *(PasswordText->GetText().ToString()));
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void UTitileUI::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool wasSuccess)
{
	UE_LOG(LogClass, Warning, TEXT("Request : %s"), *Request->GetURL());
	UE_LOG(LogClass, Warning, TEXT("Response : %s"), *Response->GetContentAsString());
	//if (Response->GetContentAsString().Equals(TEXT("SUCCESS")))
	//{
	//	UE_LOG(LogClass, Warning, TEXT("로그인 성공!"));
	//}
	//else
	//{
	//	UE_LOG(LogClass, Warning, TEXT("로그인 실패."));
	//}
	
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	bool IsSuccess = FJsonSerializer::Deserialize(Reader, JsonObj);
	if (IsSuccess)
	{
		if (JsonObj->GetStringField(TEXT("result")).Equals(TEXT("SUCCESS")))
		{
			UE_LOG(LogClass, Warning, TEXT("Good %s"), *JsonObj->GetStringField(TEXT("nick")));
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("JsonFail"));
		}
	}
}
