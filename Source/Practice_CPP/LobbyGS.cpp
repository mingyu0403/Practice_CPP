// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPC.h"

#include "Kismet/GameplayStatics.h"

void ALobbyGS::InitCountDownNum()
{
	SetCountDownNum(30);
}

bool ALobbyGS::CountDown()
{
	SetCountDownNum(CountDownNum - 1);

	return CountDownNum > 0;
}

void ALobbyGS::SetCountDownNum(int32 Num)
{
	CountDownNum = Num;
	// ������ �ƴ� �ٸ� ������ ȣ�� �� ���ɼ��� �ִٸ�, if (HasAuthority()) �˻��ؾ� ��.
	OnRep_CountDownNum(); // Replicate�� ��, ���� �ڽ��� �̺�Ʈ�� �� ����.�������Ʈ������ ���ִµ�, C++������ ������ �̷��� ȣ������� ��.
}

void ALobbyGS::SetCurrentPlayerCounts(int32 Counts)
{
	CurrentPlayerCounts = Counts;
	OnRep_CurrentPlayerCounts();
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, CountDownNum);
	DOREPLIFETIME(ALobbyGS, CurrentPlayerCounts);
}

void ALobbyGS::OnRep_CountDownNum()
{
	ALobbyPC* _PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (_PC)
	{
		_PC->UpdateNotifyMessage();
	}
}

void ALobbyGS::OnRep_CurrentPlayerCounts()
{
	ALobbyPC* _PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (_PC)
	{
		_PC->UpdataCurrentPlayerCountsMessage();
	}
}
