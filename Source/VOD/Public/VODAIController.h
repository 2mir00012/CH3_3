#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "VODAIController.generated.h"

// 전방 선언
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UUserWidget;

UCLASS()
class VOD_API AVODAIController : public AAIController
{
	GENERATED_BODY()

public:
	// 생성자
	AVODAIController();
	// AI 감지 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;
	// 시야 감지 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;
	// 현재 추적 대상
	UPROPERTY()
	AActor* CurrentTarget = nullptr;
	// 감지 상태 변경 처리
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor,FAIStimulus Stimulus);
	// 현재 추적 여부
	bool bIsChasing = false;
	// 추적 갱신 Timer
	FTimerHandle ChaseTimer;
	// 게임 시작
	virtual void BeginPlay() override;
	// Pawn 제어 시작
	virtual void OnPossess(APawn* InPawn) override;
	// 추적 시작
	void StartChasing(AActor* Target);
	// 추적 종료
	void StopChasing();
	// 추적 위치 갱신
	void UpdateChase();

private:
	// 랜덤 위치로 이동
	void MoveToRandomLocation();
	// 랜덤 이동 Timer
	FTimerHandle RandomMoveTimer;
	// 랜덤 이동 범위
	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius = 1000.0f;
};
