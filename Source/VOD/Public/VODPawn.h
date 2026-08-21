#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VODPawn.generated.h"

UCLASS()
class VOD_API AVODPawn : public APawn
{
    GENERATED_BODY()

public:
    // 생성자
    AVODPawn();
    // 매 프레임 실행되는 함수
    virtual void Tick(float DeltaTime) override;

protected:
    // 게임 시작 시 한 번 실행
    virtual void BeginPlay() override;
    // Pawn이 이동하는 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 200.0f;
    // 이동 방향
    // 1 = 전진
    // -1 = 후진
    float MoveDirection = 1.0f;
    // 이동 방향을 바꾸기 위한 시간 측정
    float ElapsedTime = 0.0f;
};
