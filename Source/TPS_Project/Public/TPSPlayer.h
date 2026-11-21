// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_PROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Spring Arm 컴포넌트 추가
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	// 카메라
	UPROPERTY(BlueprintReadOnly)
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SniperGunComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ExplodeParticle;
	
public:
	void Turn(float Value);
	void Lookup(float Value);
	void MoveRight(float Value);
	void MoveForward(float Value);
	
	void PlayerJump();
	void PlayerFire();
	void Aim();
	void ReleaseAim();
	
	float TargetFov = 90.0f;
	
	
	//crosshair UI
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<class UUserWidget> CrosshairUIClass;
	
	// crosshair instance
	UPROPERTY()
	class UUserWidget* CrosshairUI;
	
	//sniper UI
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<class UUserWidget> SniperUIClass;
	
	// sniper instance
	UPROPERTY()
	class UUserWidget* SniperUI;
};
