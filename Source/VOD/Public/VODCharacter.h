#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "VODCharacter.generated.h"

// 전방 선언
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

// Enhanced Input 값
struct FInputActionValue;

UCLASS()
class VOD_API AVODCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // 생성자
    AVODCharacter();
    // 머리 위 HP Widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* OverheadWidget;
    // 현재 체력 반환
    UFUNCTION(BlueprintPure, Category = "Health")
    int32 GetHealth() const;
    // 최대 체력 반환
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const;
    // 체력 회복
    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float Amount);

protected:
    // 게임 시작
    virtual void BeginPlay() override;
    // AI 감지 대상 등록 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionStimuliSourceComponent* StimuliSource;
    // 카메라 거리 조절
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;
    // 플레이어 카메라
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;
    // 기본 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalSpeed;
    // 스프린트 속도 배율
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeedMultiplier;
    // 실제 스프린트 속도
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float SprintSpeed;
    // 점프 횟수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
    int32 JumpCount;
    // 마우스 감도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MouseSensitivity;
    // 최대 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;
    // 현재 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;
    // 사망 처리
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void OnDeath();
    // 머리 위 HP UI 갱신
    void UpdateOverheadHP();
    // 데미지 처리
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) override;
    // 입력 바인딩
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    // 이동
    UFUNCTION()
    void Move(const FInputActionValue& Value);
    // 점프 시작
    UFUNCTION()
    void StartJump(const FInputActionValue& Value);
    // 점프 종료
    UFUNCTION()
    void StopJump(const FInputActionValue& Value);
    // 카메라 회전
    UFUNCTION()
    void Look(const FInputActionValue& Value);
    // 스프린트 시작
    UFUNCTION()
    void StartSprint(const FInputActionValue& Value);
    // 스프린트 종료
    UFUNCTION()
    void StopSprint(const FInputActionValue& Value);
};
