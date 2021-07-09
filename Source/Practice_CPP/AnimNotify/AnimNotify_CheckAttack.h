// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CheckAttack.generated.h"

UCLASS()
class PRACTICE_CPP_API UAnimNotify_CheckAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	virtual FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Offset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;
};
