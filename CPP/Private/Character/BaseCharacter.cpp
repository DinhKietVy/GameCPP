#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAsset/EnhancedInputData.h"
#include "DataAsset/BaseCharacterData.h"
#include "Component/AttackComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	//spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->AddLocalOffset(FVector(0.0f,0.0f,40.0f));
	//camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	bUseControllerRotationYaw = false;
	//attack component
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 540.0;
}
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddMappingContextForCharacter();

	//bind input actions
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputData == nullptr)
		return;

	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
	EnhancedInputComponent->BindAction(EnhancedInputData->IA_Attack, ETriggerEvent::Started, this, &ABaseCharacter::Attack);

}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//attacck component
	if (AttackComponent) {
		AttackComponent->HitSomethingDelegate.BindDynamic(this, &ABaseCharacter::HandleHitSomething);
		AttackComponent->SetupAttackComponent(BaseCharacterData);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//bind delegate	nhan sat thuong
	OnTakePointDamage.AddDynamic(this, &ABaseCharacter::HandleTakePointDamage);
}

#pragma region Attack_Interface

void ABaseCharacter::I_PlayAttackMontage(UAnimMontage* AttackMontage)
{
	PlayAnimMontage(AttackMontage);
}

void ABaseCharacter::I_AN_EndAttack()
{
	//isattaking->false
	if(AttackComponent)
	AttackComponent->AN_EndAttack();
}

FVector ABaseCharacter::I_GetSocketLocation(const FName SocketName) const
{
	if (GetMesh() == nullptr)
		return FVector();
	return GetMesh()->GetSocketLocation(SocketName);
}
void ABaseCharacter::I_ANS_BeginTraceHit()
{
	if (AttackComponent)
		AttackComponent->SetupTraceHit();
}

void ABaseCharacter::I_ANS_TraceHit()
{
	//line trace
	if (AttackComponent)
		AttackComponent->TraceHit();
}

#pragma endregion


void ABaseCharacter::AddMappingContextForCharacter()
{
	//player controller
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr)
		return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	//input mapping context
	if (EnhancedInputData && Subsystem)
		Subsystem->AddMappingContext(EnhancedInputData->MappingContext, 0);
}


void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	//trai phai
	if(LookValue.X!=0.0)
	AddControllerYawInput(LookValue.X);

	if (LookValue.Y != 0.0)
	AddControllerPitchInput(LookValue.Y);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();

	FRotator MyControllerRotation = FRotator(0.0,GetControlRotation().Yaw,0.0);

	if (MoveValue.Y != 0)
	AddMovementInput(MyControllerRotation.RotateVector(FVector::ForwardVector), MoveValue.Y);

	if(MoveValue.X!=0)
	AddMovementInput(MyControllerRotation.RotateVector(FVector::RightVector),MoveValue.X);
}

void ABaseCharacter::Attack()
{
	if(AttackComponent)
		AttackComponent->RequesAttack();
}

void ABaseCharacter::HandleHitSomething(const FHitResult& HitResult)
{
	if (BaseCharacterData == nullptr)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan,TEXT("Handle Hit Something"));

	//apply point damage
	//vi tri nguoi tan cong - den nan nhan
	auto HitActor = HitResult.GetActor();
	if (HitActor == nullptr)
		return;

	const auto AttackDirection =
	UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), HitActor->GetActorLocation());

	UGameplayStatics::ApplyPointDamage(HitActor,BaseCharacterData->Damage,
		AttackDirection, HitResult,GetController(),this,UDamageType::StaticClass());
}

void ABaseCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Handle Take Point Damage"));
	//hit react animation montage
	if (BaseCharacterData) {
		//get correct hit react montage
		PlayAnimMontage(GetCorrectHitReactMontage(ShotFromDirection));
		CombatState = ECombatState::Beaten;
	}
	
}

UAnimMontage* ABaseCharacter::GetCorrectHitReactMontage(const FVector& AttackDirecton) const
{
	if (BaseCharacterData == nullptr) return nullptr;

	//front back montage -> dot product
	//left right montage -> cross product
	auto Cross = FVector::CrossProduct(AttackDirecton,GetActorForwardVector());
	double Dot = FVector::DotProduct(AttackDirecton, GetActorForwardVector());
	//print Dot Produc
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Blue,
			FString::Printf(TEXT("Dot = %f"), Dot)
		);
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Blue,
			FString::Printf(TEXT("Cross = %f %f %f"), Cross.X,Cross.Y,Cross.Z)
		);
	}
	if (Dot > 0.5 && Dot < 1) {
		return BaseCharacterData->HitReactMontage_Back;
	}
	else if(Dot>-1 && Dot <-0.5) {
		return BaseCharacterData->HitReactMontage_Front;
	}
	
	if (Cross.Z > 0) {
		return BaseCharacterData->HitReactMontage_Right;
	}
	else
		return BaseCharacterData->HitReactMontage_Left;
		
}