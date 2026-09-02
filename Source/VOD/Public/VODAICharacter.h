#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VODAICharacter.generated.h"

// 좀비 종류
UENUM(BlueprintType)
enum class EZombieType : uint8
{
	HumanZombie UMETA(DisplayName = "HumanZombie"),
	DogZombie UMETA(DisplayName = "DogZombie"),
};

UCLASS()
class VOD_API AVODAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 생성자
	AVODAICharacter();
	// 이동 속도 변경
	void SetMovementSpeed(float NewSpeed);
	// 걷기 속도
	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 300.0f;
	// 추적 시 달리기 속도
	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 600.0f;
	// 좀비 종류
	UPROPERTY(EditAnywhere)
	EZombieType zombieType;

protected:
	// 게임 시작
	virtual void BeginPlay() override;
};