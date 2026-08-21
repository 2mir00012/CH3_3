#include "MyPlayerController.h"

// Enhanced Input Subsystem 사용
#include "EnhancedInputSubsystems.h"
// Enhanced InputComponent 사용
#include "EnhancedInputComponent.h"
// FInputActionValue 사용
#include "InputActionValue.h"

AMyPlayerController::AMyPlayerController()
    : InputMappingContext(nullptr),
    UIInputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    ToggleUIAction(nullptr),
    bIsUIMode(false)
{
}

void AMyPlayerController::BeginPlay()
{
    // 부모 PlayerController의 BeginPlay 실행
    Super::BeginPlay();
    // 현재 PlayerController와 연결된 Local Player 가져오기
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player의 Enhanced Input Subsystem 가져오기
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            // IMC가 정상적으로 지정되어 있을 경우
            if (InputMappingContext)
            {
                // IMC 활성화
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}
void AMyPlayerController::SetupInputComponent()
{
    // 부모 PlayerController의 입력 설정 실행
    Super::SetupInputComponent();
    // 기본 InputComponent를
    // Enhanced InputComponent로 변환
    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(InputComponent))
    {
        // IA_ToggleUI가 정상적으로 지정되어 있는지 확인
        if (ToggleUIAction)
        {
            // IA_ToggleUI 입력이 시작되는 순간
            // ToggleMappingContext 함수 실행
            EnhancedInputComponent->BindAction(
                ToggleUIAction, ETriggerEvent::Started, this, &AMyPlayerController::ToggleMappingContext);
        }
    }
}


// ================================
// IMC 전환
// ================================

void AMyPlayerController::ToggleMappingContext(const FInputActionValue & Value)
{
    // 현재 PlayerController의 Local Player 가져오기
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Enhanced Input Subsystem 가져오기
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {

            // --------------------------------
            // 현재 UI 모드일 경우
            // --------------------------------

            if (bIsUIMode)
            {
                // UI용 IMC 제거
                if (UIInputMappingContext)
                {
                    Subsystem->RemoveMappingContext(UIInputMappingContext);
                }
                // 캐릭터 조작용 IMC 다시 활성화
                if (InputMappingContext)
                {
                    Subsystem->AddMappingContext(InputMappingContext, 0);
                }
                // 현재 상태를 캐릭터 모드로 변경
                bIsUIMode = false;
                // 테스트용 로그
                UE_LOG(LogTemp, Warning, TEXT("캐릭터"));
            }

            // --------------------------------
            // 현재 캐릭터 모드일 경우
            // --------------------------------

            else
            {
                // 캐릭터 조작용 IMC 제거
                if (InputMappingContext)
                {
                    Subsystem->RemoveMappingContext(InputMappingContext);
                }
                // UI용 IMC 활성화
                if (UIInputMappingContext)
                {
                    Subsystem->AddMappingContext(UIInputMappingContext, 0);
                }
                // 현재 상태를 UI 모드로 변경
                bIsUIMode = true;
                // 테스트용 로그
                UE_LOG(LogTemp, Warning, TEXT("UI"));
            }
        }
    }
}