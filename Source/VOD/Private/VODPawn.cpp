#include "VODPawn.h"

AVODPawn::AVODPawn()
{
    // Tick 활성화
    PrimaryActorTick.bCanEverTick = true;
}
void AVODPawn::BeginPlay()
{
    Super::BeginPlay();
}
void AVODPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 시간 누적
    ElapsedTime += DeltaTime;
    // 2초마다 이동 방향 반전
    if (ElapsedTime >= 2.0f)
    {
        MoveDirection *= -1.0f;
        // 시간 초기화
        ElapsedTime = 0.0f;
    }
    // 프레임당 이동 거리 계산
    FVector MoveOffset(MoveSpeed * MoveDirection * DeltaTime, 0.0f, 0.0f);
    // Local X축으로 이동
    AddActorLocalOffset(MoveOffset);
}
