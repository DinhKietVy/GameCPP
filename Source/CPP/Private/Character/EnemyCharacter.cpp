// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

//set gia tri cho key patrol
//ai controller -> blackboard
FVector AEnemyCharacter::I_GetPatrolLocation()
{
    if (PatrolPoint.IsEmpty()) return GetActorLocation();

    if (PatrolPoint[PatrolIndex] == nullptr) return GetActorLocation();

    auto RetrunValue = PatrolPoint[PatrolIndex]->GetActorLocation();
    PatrolIndex = (PatrolIndex + 1) % PatrolPoint.Num();
    return RetrunValue;
}
