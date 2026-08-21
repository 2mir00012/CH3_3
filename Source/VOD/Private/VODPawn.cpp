#include "VODPawn.h"

AVODPawn::AVODPawn()
{
    // Tick을 사용해야 하므로 true
    PrimaryActorTick.bCanEverTick = true;
}

void AVODPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AVODPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ================================
    // 1. 시간 누적
    // ================================

    // 매 프레임 DeltaTime을 계속 더함
    ElapsedTime += DeltaTime;

    // ================================
    // 2. 2초마다 이동 방향 변경
    // ================================

    if (ElapsedTime >= 2.0f)
    {
        // 1이면 -1
        // -1이면 다시 1이 됨
        MoveDirection *= -1.0f;
        // 시간 초기화
        ElapsedTime = 0.0f;
    }

    // ================================
    // 3. 이동할 거리 계산
    // ================================

    FVector MoveOffset(MoveSpeed * MoveDirection * DeltaTime, 0.0f, 0.0f);

    // ================================
    // 4. Pawn 이동
    // ================================

    // 자신의 Local X축 방향으로 이동
    AddActorLocalOffset(MoveOffset);
}

