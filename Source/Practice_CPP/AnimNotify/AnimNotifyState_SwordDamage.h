// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SwordDamage.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API UAnimNotifyState_SwordDamage : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;

private:
	TArray<AActor*> IgnoreActor;
};
