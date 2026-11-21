// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_PROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Anim")
	float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Anim")
	float Direction;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Anim")
	bool IsInAir;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Player Anim")
	void PlayFireAnimation();
};
