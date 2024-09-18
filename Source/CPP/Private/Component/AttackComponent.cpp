// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Interface/AttackInterface.h"
#include "DataAsset/BaseCharacterData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackInterface = TScriptInterface<IAttackInterface>(GetOwner());
	
}
void UAttackComponent::RequesAttack()
{
	//khong tan cong || co the combo -> attack
	const bool bCanAttack = bIsAttacking == false || bCanCombo == true;

	if (bCanAttack)
		Attack();
	else bSaveAttack = true;
}

void UAttackComponent::TraceHit()
{
	if (AttackInterface == nullptr)
		return;

	const FVector& StartLocation = AttackInterface->I_GetSocketLocation(BaseCharacterData->TraceStart);
	const FVector& EndLocation = AttackInterface->I_GetSocketLocation(BaseCharacterData->TraceEnd);
	//Hit result
	TArray<FHitResult> HitResult;

	if (BaseCharacterData == nullptr)
		return;

	auto DrawType = BaseCharacterData->bDrawDebugTrace ?
		EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bDoHitSomeThing = UKismetSystemLibrary::SphereTraceMultiForObjects(this,
		StartLocation,
		EndLocation, BaseCharacterData->TraceRadius,
		BaseCharacterData->TraceObjectTypes,
		false,
		BaseCharacterData->ActorsToIgnore,
		DrawType, HitResult, true, FLinearColor::Red,
		FLinearColor::Green, BaseCharacterData->DrawTime);

	if (bDoHitSomeThing == false)
		return;

	
	for (const FHitResult& Result : HitResult) {
		if (HittedActor.Contains(Result.GetActor())) continue;

		HandleHitResult(Result);
		//add->emplace
		HittedActor.Emplace(Result.GetActor());
	}
}

void UAttackComponent::HandleHitResult(const FHitResult& Result)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, Result.BoneName.ToString());
	//character->dang ky delegate
	//character-> apply point damage
	//thong bao danh trung muc tieu, truyen vao hit result
	if(HitSomethingDelegate.IsBound())
		HitSomethingDelegate.Execute(Result);
}

UAnimMontage* UAttackComponent::GetCorrectAttackMontage()
{
	if (BaseCharacterData == nullptr) return nullptr;
	if (BaseCharacterData->AttackMontages.IsEmpty()) return nullptr;

	return BaseCharacterData->AttackMontages[AttackIndex];
}

void UAttackComponent::Attack()
{
	if (AttackInterface && BaseCharacterData && GetCorrectAttackMontage())
	{
		AttackInterface->I_PlayAttackMontage(GetCorrectAttackMontage());
		//attackinterface -> play start attack sound
		AttackInterface->I_PlayStartAttackSound();
		bIsAttacking = true;
		bCanCombo = false;
		AttackIndex = (AttackIndex +1) % BaseCharacterData->AttackMontages.Num();
	}
}

void UAttackComponent::SetupAttackComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;
}

void UAttackComponent::AN_EndAttack()
{
	bIsAttacking = false;
	bCanCombo = false;
	bSaveAttack = false;
	AttackIndex = 0;
}

void UAttackComponent::AN_Combo()
{
	//bcancombo
	bCanCombo = true;

	if (bSaveAttack) {
		RequesAttack();
		bSaveAttack = false;
	}
}

void UAttackComponent::SetupTraceHit()
{
	HittedActor.Empty();
}