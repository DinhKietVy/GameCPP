// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstace/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enum/CombatState.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//character
	//movement component
	//speed
	if (MovementComponent == nullptr || Character == nullptr)
		return;

	GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);

	IsFalling = MovementComponent->IsFalling();

	bShouldMove = GroundSpeed > 0;
	//blend lower upper?
	//dang di chuyen && khong choi hit react animation
	//base character->luu trang thai nhan vat
	//combat state -> ready, beaten
	//enum
	bShoulBlendLowerUpper = GroundSpeed > 0 && Character->GetCombatState() != ECombatState::Beaten;
}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	//pawn->character
	Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if(Character)
		MovementComponent = Character->GetCharacterMovement();
}

