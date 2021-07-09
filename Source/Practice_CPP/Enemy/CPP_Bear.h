#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_BearAnim.h"
#include "CPP_Bear.generated.h"

class UCPP_BearAnim;
class UPawnSensingComponent;
class ATargetPoint;
class UWidgetComponent;

UCLASS()
class PRACTICE_CPP_API ACPP_Bear : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_Bear();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(VisibleAnywhere)
	UCPP_BearAnim* AnimBP;
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	EBearState CurrentState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPawnSensingComponent* PawnSensing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWidgetComponent* Widget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=WayPoints)
	TArray<ATargetPoint*> WayPoints;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
	float MaxHP = 1500.0f;
	UPROPERTY(Replicated, BlueprintReadWrite, Editanywhere)
	float CurrentHP;

public:
	void SetTarget(AActor* Actor);

	UFUNCTION(NetMulticast, UnReliable)
	void MC_CheckHP(float HP);
	void BillboardUI();

	UFUNCTION(NetMulticast, UnReliable)
	void MC_SetCurrentState(EBearState state);
	void SetHPProgress();

	UFUNCTION(NetMulticast, UnReliable)
	void MC_SetAttackTrigger(bool isOn);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
};
