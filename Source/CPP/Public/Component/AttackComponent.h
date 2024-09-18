// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

//hit result
//delegate type
DECLARE_DYNAMIC_DELEGATE_OneParam(FHitSomethingDelegate, const FHitResult&, HitResult);

class UAnimMontage;
class UBaseCharacterData;
class ACharacter;
class IAttackInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RequesAttack();
	void TraceHit();
	
	void SetupAttackComponent(UBaseCharacterData* BCD);
	void AN_EndAttack();
	void AN_Combo();
	void SetupTraceHit();

private:
	void Attack();
	void HandleHitResult(const FHitResult& Result);
	UAnimMontage* GetCorrectAttackMontage();
public:
	FHitSomethingDelegate HitSomethingDelegate;
protected:

	virtual void BeginPlay() override;

private:
	//luu actor co trien khai interface
	UPROPERTY()
	TScriptInterface<IAttackInterface> AttackInterface;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;

	bool bIsAttacking = false;
	bool bCanCombo = false;
	bool bSaveAttack = false;
	int AttackIndex = 0;

	//trace hit
	TArray<AActor*> HittedActor;

};
