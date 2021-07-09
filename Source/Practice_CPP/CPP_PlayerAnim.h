#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_PlayerAnim.generated.h"

class UAnimMontage;
class ACPP_Player;

UCLASS()
class PRACTICE_CPP_API UCPP_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCPP_PlayerAnim();
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="state")
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "state")
	float ModelAimPitch;

	UPROPERTY(VisibleAnywhere, Category = "SwordAttack")
	int ComboIndex;
	UPROPERTY(VisibleAnywhere, Category = "SwordAttack")
	bool bIsSwordMotion;
	UPROPERTY(VisibleAnywhere, Category = "SwordAttack")
	bool bIsShotMotion;
	UPROPERTY(VisibleAnywhere)
	bool bUsingIronSight;
	UPROPERTY(VisibleAnywhere)
	bool bIsRollingMotion;

	UPROPERTY(VisibleAnywhere)
	ACPP_Player* Player;

	UAnimMontage* SwordAttack0;
	UAnimMontage* SwordAttack1;
	UAnimMontage* SwordAttack2;
	UAnimMontage* AimPose;
	UAnimMontage* GunAttack0;
	UAnimMontage* Rolling0;
	UAnimMontage* Dead0;

public:
	void SwordAttack();
	void GunAttack();
	void Rolling();
	void ResetMotion();
	void Dead();

	UFUNCTION()
	void AnimNotify_EndAttack();
	UFUNCTION()
	void AnimNotify_EndCombo();
	UFUNCTION()
	void AnimNotify_EndShot();
	UFUNCTION()
	void AnimNotify_RollingEnd();
	UFUNCTION()
	void AnimNotify_GhostTail();
};
