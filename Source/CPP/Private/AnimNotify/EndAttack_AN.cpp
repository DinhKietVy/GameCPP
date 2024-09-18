// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/EndAttack_AN.h"
#include "Interface/AttackInterface.h"

void UEndAttack_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//owner->attack interface
	//interface->an end attack
	if (MeshComp == nullptr)
		return;

	TScriptInterface<IAttackInterface> AttackInterface = TScriptInterface<IAttackInterface>(MeshComp->GetOwner());

	if (AttackInterface)
		AttackInterface->I_AN_EndAttack();
}
