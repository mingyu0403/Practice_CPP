// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UCPP_PlayerAnim;
class UParticleSystem;
class UMaterialInstance;

UCLASS()
class PRACTICE_CPP_API ACPP_Player : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_Player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Gun;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Sword;
	UPROPERTY(VisibleAnywhere)
	UCPP_PlayerAnim* AnimBP;

	UPROPERTY(BlueprintReadWrite, Editanywhere, category="AIM", Replicated)
	bool bIsIronSight;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category="AIM")
	float NormalFOV = 90.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category="AIM")
	float AimFOV = 65.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	float NormalSpringArmLength = 500.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	float AimSpringArmLength = 100.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	FVector NormalSocketOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	FVector AimSocketOffset = FVector(0.0f, 35.0f, 60.0f);
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM", Replicated)
	float AimPitch;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	float NormalWalkSpeed = 600.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, category = "AIM")
	float AimWalkSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, Editanywhere)
	float MaxHP = 100.0f;
	UPROPERTY(BlueprintReadWrite, Editanywhere, Replicated)
	float CurrentHP;

public:
	UFUNCTION()
	void OnTurn(float AxisValue);
	UFUNCTION()
	void OnLookUp(float AxisValue);
	UFUNCTION()
	void OnMoveForward(float AxisValue);
	UFUNCTION()
	void OnMoveRight(float AxisValue);
	UFUNCTION()
	void OnAttack();
	
	UFUNCTION(Server, UnReliable)
	void CtoS_Rolling();
	UFUNCTION(NetMulticast, UnReliable)
	void MC_Rolling(FVector LastInputVector);
	
	UFUNCTION()
	void OnIronSight_Press();
	UFUNCTION()
	void OnIronSight_Release();
	
	UFUNCTION(Server, UnReliable)
	void CtoS_SwordAttack();
	UFUNCTION(NetMulticast, UnReliable)
	void MC_SwordAttack();
	UFUNCTION(Server, UnReliable)
	void CtoS_GunAttack();
	UFUNCTION(NetMulticast, UnReliable)
	void MC_GunAttack(FVector TargetLoc, FVector MuzzleLoc);
	UFUNCTION(Server, UnReliable)
	void CtoS_SetIronSight(bool IsOn);
	UFUNCTION(NetMulticast, UnReliable)
	void MC_SetIronSight(bool IsOn);
	UFUNCTION(Server, UnReliable)
	void CtoS_SetAimPitch();
	UFUNCTION(NetMulticast, UnReliable)
	void MC_SetAimPitch(float NewAimPitch);
	UFUNCTION()
	void RollingEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SpawnGhostTail();

	UFUNCTION(Server, UnReliable)
	void CtoS_CheckHP(float HP);
	UFUNCTION(NetMulticast, UnReliable)
	void MC_CheckHP(float HP);

private:
	FVector CheckTargetAtCamera();
	void SpawnGunEffect(FVector& StartLoc, FVector& TargetLoc);
	void SpawnHitEffect(FHitResult& HitResult);
	UParticleSystem* LaserParticle;
	UParticleSystem* BloodParticle;
	UMaterialInstance* HitDecal;
};
