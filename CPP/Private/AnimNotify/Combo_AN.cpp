// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Combo_AN.h"

void UCombo_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
}
