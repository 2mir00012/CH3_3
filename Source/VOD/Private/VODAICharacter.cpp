#include "VODAICharacter.h"
#include "VODAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AVODAICharacter::AVODAICharacter()
{
	// 사용할 AI Controller 지정
	AIControllerClass = AVODAIController::StaticClass();
	// 배치되거나 Spawn되었을 때 자동 Possess
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// Character Movement 가져오기
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	// HumanZombie 이동 속도
	if (zombieType == EZombieType::HumanZombie)
	{
		Movement->MaxWalkSpeed = WalkSpeed * 1;
	}
	// DogZombie 이동 속도
	else if (zombieType == EZombieType::DogZombie)
	{
		Movement->MaxWalkSpeed = WalkSpeed * 2;
	}
	// 이동 방향으로 캐릭터 회전
	Movement->bOrientRotationToMovement = true;
	// 회전 속도
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}
void AVODAICharacter::BeginPlay()
{
	Super::BeginPlay();
	// AI 생성 확인
	UE_LOG(LogTemp, Warning, TEXT("[VOD] AI character has been spawned."));
}
void AVODAICharacter::SetMovementSpeed(float NewSpeed)
{
	// 이동 속도 변경
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("[VOD] Speed changed: %.1f"), NewSpeed);
	}
}