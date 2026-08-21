#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

// Input Mapping Context 전방 선언
class UInputMappingContext;
// Input Action 전방 선언
class UInputAction;
// Input Action의 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // 생성자
    AMyPlayerController();
    // IMC_Character를 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;
    // UI 조작용 IMC
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* UIInputMappingContext;
    // IA_Move를 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;
    // IA_Jump를 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;
    // IA_Look을 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;
    // IA_Sprint를 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction;
    // IA_ToggleUI를 연결할 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ToggleUIAction;
protected:
    // 게임 시작 시 실행
    virtual void BeginPlay() override;
    // PlayerController 입력 연결
    virtual void SetupInputComponent() override;
    // Tab을 눌렀을 때 IMC를 전환할 함수
    void ToggleMappingContext(const FInputActionValue& Value);
private:
    // 현재 UI 입력 모드인지 확인
    bool bIsUIMode;
};
