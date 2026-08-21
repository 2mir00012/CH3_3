#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

// SpringArm 클래스 전방 선언
class USpringArmComponent;
// Camera 클래스 전방 선언
class UCameraComponent;
// Enhanced Input에서 입력값을 전달받기 위한 구조체 전방 선언
struct FInputActionValue;

UCLASS()
class TEST_1_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // 생성자
    AMyCharacter();

protected:
    // 3인칭 카메라 거리를 담당하는 SpringArm
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;
    // 실제 화면을 보여주는 Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    // 이동 속도
    // 기본 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalSpeed;
    // 기본 속도의 몇 배로 스프린트할지 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeedMultiplier;
    // 최종적으로 계산된 스프린트 속도
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float SprintSpeed;
    // 점프 횟수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    int32 JumpCount;
    // 마우스 감도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MouseSensitivity;

    // 입력 바인딩
    // Input Action과 Character 함수를 연결
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // 캐릭터 동작 함수
    // WASD 이동
    UFUNCTION()
    void Move(const FInputActionValue& Value);
    // 점프 시작
    UFUNCTION()
    void StartJump(const FInputActionValue& Value);
    // 점프 종료
    UFUNCTION()
    void StopJump(const FInputActionValue& Value);
    // 마우스 시점 회전
    UFUNCTION()
    void Look(const FInputActionValue& Value);
    // 스프린트 시작
    UFUNCTION()
    void StartSprint(const FInputActionValue& Value);
    // 스프린트 종료
    UFUNCTION()
    void StopSprint(const FInputActionValue& Value);
};
