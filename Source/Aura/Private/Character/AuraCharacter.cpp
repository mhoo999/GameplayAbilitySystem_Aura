// Copyright Hoon


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 이동 방향으로 캐릭터 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 캐릭터 이동 회전반경 제어
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// 캐릭터의 이동을 평면으로 제한
	GetCharacterMovement()->bConstrainToPlane = true;
	// 캐릭터를 평면에 스냅
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 캐릭터가 컨트롤러의 회전을 사용하지 않도록 셋팅
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
