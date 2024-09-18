// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetSystemLibrary.h"

//AI Module
AEnemyAIController::AEnemyAIController()
{
	//ai perception component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));

	if (AISightConfig == nullptr) return;

	AISightConfig->SightRadius = 2500.0f;
	AISightConfig->LoseSightRadius = 2500.0f;
	//110 do
	AISightConfig->PeripheralVisionAngleDegrees = 55.0f;
	AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*AISightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedPawn = InPawn;
	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);

	RunBehaviorTree(BehaviorTree);
	if(AIPerceptionComponent)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,
		&AEnemyAIController::HandleTargetPerceptionUpdated);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//draw debug cone -> thu vien kismet
	if (PossessedPawn == nullptr) return;

	UKismetSystemLibrary::DrawDebugCone(this, PossessedPawn->GetActorLocation(),
		PossessedPawn->GetActorForwardVector(),
		2500.0f,
		FMath::DegreesToRadians(55.0f),
		FMath::DegreesToRadians(55.0f),
		20, DebugColor,-1.0f,3.0f);
}



void AEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("See Player"));
		DebugColor = FLinearColor::Red;
	}
	else {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Lose Sight Player"));
		DebugColor = FLinearColor::Green;
	}
}



void AEnemyAIController::UpdatePatrolLocation()
{
	//key patrol location
	if(Blackboard&&EnemyInterface)
	Blackboard->SetValueAsVector(Key_PatrolLocation, EnemyInterface->I_GetPatrolLocation());
}
