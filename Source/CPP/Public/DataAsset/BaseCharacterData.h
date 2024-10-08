// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseCharacterData.generated.h"

class UAnimMontage;
class UParticleSystem;

UCLASS()
class CPP_API UBaseCharacterData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray < UAnimMontage*> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float TraceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	FName TraceStart;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	FName TraceEnd;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float DrawTime=1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	bool bDrawDebugTrace = true;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Damage = 20.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Hit React Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React Montage")
	UAnimMontage* HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React Montage")
	UAnimMontage* HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React Montage")
	UAnimMontage* HitReactMontage_Left;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React Montage")
	UAnimMontage* HitReactMontage_Right;

	//Hit impact 
	UPROPERTY(EditDefaultsOnly, Category = "Hit Impact")
	UParticleSystem* HitImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* HitImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* PainSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* StartAttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;

};
