// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// owner pawn 가져오기
	auto pawn = TryGetPawnOwner();
	// 캐스팅
	ATPSPlayer* player = Cast<ATPSPlayer>(pawn);
	if (player)
	{
		// 스피드, 디렉션, 에어
		Speed = player->GetVelocity().Dot(player->GetActorForwardVector());
		Direction = player->GetVelocity().Dot(player->GetActorRightVector());
		IsInAir = player->GetCharacterMovement()->IsFalling();
	}
	
}
