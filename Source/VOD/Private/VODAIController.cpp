#include "VODAIController.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "VODAICharacter.h"
#include "Kismet/GameplayStatics.h"

AVODAIController::AVODAIController()
{
	// Perception 컴포넌트 생성
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	// Sight 설정 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	// 시야 거리 설정
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 2000.0f;
	// 시야 각도
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	// 감지 정보 유지 시간
	SightConfig->SetMaxAge(5.0f);
	// 모든 진영 감지
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	// Sight 등록
	AIPerception->ConfigureSense(*SightConfig);
	// 기본 감각을 Sight로 설정
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}
void AVODAIController::BeginPlay()
{
	Super::BeginPlay();
	// 감지 이벤트 연결
	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AVODAIController::OnPerceptionUpdated);
	}
	// 랜덤 이동 시작
	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &AVODAIController::MoveToRandomLocation, 3.0f, true,1.0f);
}
void AVODAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// Possess한 Pawn 확인
	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VOD] AI Controller is controlling %s."), *InPawn->GetName());
	}
}
void AVODAIController::MoveToRandomLocation()
{
	// 제어 중인 Pawn 가져오기
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Error,TEXT("[VOD] No Pawn to control."));
	}
	// Navigation System 가져오기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("[VOD] Could not find Navigation System."));
	}
	// 랜덤 위치 저장
	FNavLocation RandomLocation;
	// 이동 가능한 위치 검색
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius( MyPawn->GetActorLocation(), MoveRadius, RandomLocation);
	// 랜덤 위치로 이동
	if (bFoundLocation)
	{
		MoveToLocation(RandomLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("[VOD] Move target: %s"),*RandomLocation.Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[VOD] Could not find a reachable location."));
	}
}
void AVODAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Player Pawn 가져오기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// Player가 아니면 무시
	if (Actor != PlayerPawn)
	{
		return;
	}
	// 플레이어 감지 성공
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("[VOD] Saw something! %s"), *Actor->GetName());
		// 감지 위치 표시
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("Saw: %s"), *Actor->GetName()), nullptr, FColor::Green, 2.0f, true);
		// 추적 시작
		StartChasing(Actor);
	}
	else
	{
		// 플레이어를 놓친 경우
		UE_LOG(LogTemp, Warning, TEXT("[VOD] Missed it! %s"), *Actor->GetName());
		DrawDebugString(GetWorld(), Actor->GetActorLocation() + FVector(0, 0, 100), FString::Printf( TEXT("Missed: %s"), *Actor->GetName()), nullptr, FColor::Red, 2.0f, true);
		// 추적 종료
		StopChasing();
	}
}
void AVODAIController::StartChasing(AActor* Target)
{
	// 같은 대상을 이미 추적 중이면 종료
	if (bIsChasing && CurrentTarget == Target)
	{
		return;
	}
	// 추적 대상 저장
	CurrentTarget = Target;
	// 추적 상태 활성화
	bIsChasing = true;
	// 랜덤 이동 중지
	GetWorldTimerManager().ClearTimer(RandomMoveTimer);
	// 달리기 속도로 변경
	if (AVODAICharacter* AIChar = Cast<AVODAICharacter>(GetPawn()))
	{
		AIChar->SetMovementSpeed(AIChar->RunSpeed);
	}
	// 즉시 추적
	UpdateChase();
	// 추적 위치 반복 갱신
	GetWorldTimerManager().SetTimer(ChaseTimer, this, &AVODAIController::UpdateChase, 0.25f, true);
}
void AVODAIController::StopChasing()
{
	// 추적 중이 아니라면 종료
	if (!bIsChasing)
	{
		return;
	}
	// 추적 대상 초기화
	CurrentTarget = nullptr;
	// 추적 상태 종료
	bIsChasing = false;
	// 추적 Timer 종료
	GetWorldTimerManager().ClearTimer(ChaseTimer);
	// 현재 이동 중지
	StopMovement();
	// 걷기 속도로 복귀
	if (AVODAICharacter* AIChar = Cast<AVODAICharacter>(GetPawn()))
	{
		AIChar->SetMovementSpeed(AIChar->WalkSpeed);
	}
	// 랜덤 이동 재시작
	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &AVODAIController::MoveToRandomLocation, 3.0f, true, 2.0f);
}
void AVODAIController::UpdateChase()
{
	// 추적 대상에게 이동
	if (CurrentTarget && bIsChasing)
	{
		MoveToActor(CurrentTarget, 100.0f);
	}
}