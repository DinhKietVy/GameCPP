// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABaseCharacter;

UCLASS()
class CPP_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling = false;

	UPROPERTY(BlueprintReadOnly)
	bool bShoulBlendLowerUpper = true;
private:
	UPROPERTY()
	ABaseCharacter* Character;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	float GroundSpeed = 0.0f;
};
