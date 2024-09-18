// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class IEnemyInterface;

UCLASS()
class CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePatrolLocation();
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UPROPERTY()
	APawn* PossessedPawn;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISightConfig;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	FName Key_PatrolLocation;

	UPROPERTY()
	TScriptInterface<IEnemyInterface> EnemyInterface;

	FLinearColor DebugColor= FLinearColor::Green;
};
