#include "VODPlayerController.h"
#include "VODGameState.h"
#include "VODGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AVODPlayerController::AVODPlayerController()
    : InputMappingContext(nullptr),
    UIInputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    ToggleUIAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr),
    bIsUIMode(false)
{
}
void AVODPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // Enhanced Input Subsystem 가져오기
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            // 캐릭터 Mapping Context 등록
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
    // 현재 Map 이름 가져오기
    FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
    // MenuLevel에서는 Main Menu 표시
    if (CurrentMapName.Contains(TEXT("MenuLevel")))
    {
        ShowMainMenu(false, false);
    }
}
UUserWidget* AVODPlayerController::GetHUDWidget() const
{
    // 생성된 HUD 반환
    return HUDWidgetInstance;
}
void AVODPlayerController::ShowMainMenu(bool bIsRestart, bool bIsGameOver, bool bIsGameClear)
{
    // 기존 HUD 제거
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    // 기존 Main Menu 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    // Main Menu 생성
    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            // 화면에 표시
            MainMenuWidgetInstance->AddToViewport();
            // 마우스 표시
            bShowMouseCursor = true;
            // UI 입력 모드 설정
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
            SetInputMode(InputMode);
            // Start 버튼 상태 설정
            if (UWidget* StartButton = MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButton")))
            {
                StartButton->SetVisibility(!bIsRestart
                    ? ESlateVisibility::Visible
                    : ESlateVisibility::Collapsed
                );
                StartButton->SetIsEnabled(!bIsRestart);
            }
            // Restart 버튼 상태 설정
            if (UWidget* RestartButton = MainMenuWidgetInstance->GetWidgetFromName(TEXT("RestartButton")))
            {
                RestartButton->SetVisibility(bIsRestart
                    ? ESlateVisibility::Visible
                    : ESlateVisibility::Collapsed
                );
                RestartButton->SetIsEnabled(bIsRestart);
            }
            // Exit 버튼 상태 설정
            if (UWidget* ExitButton = MainMenuWidgetInstance->GetWidgetFromName(TEXT("ExitButton")))
            {
                // Game Over와 Game Clear에서는 숨김
                ExitButton->SetVisibility((bIsGameOver || bIsGameClear)
                    ? ESlateVisibility::Collapsed
                    : ESlateVisibility::Visible
                );
            }
            // Main Menu 버튼 상태 설정
            if (UWidget* MainMenuButton = MainMenuWidgetInstance->GetWidgetFromName(TEXT("MainMenuButton")))
            {
                // Game Over 또는 Game Clear에서 표시
                MainMenuButton->SetVisibility((bIsGameOver || bIsGameClear)
                    ? ESlateVisibility::Visible
                    : ESlateVisibility::Collapsed
                );
                // 종료 화면에서만 활성화
                MainMenuButton->SetIsEnabled(bIsGameOver || bIsGameClear);
            }
            // GameOverText 찾기
            if (UWidget* GameOverText = MainMenuWidgetInstance->GetWidgetFromName(TEXT("GameOverText")))
            {
                // Game Over일 때만 표시
                GameOverText->SetVisibility(bIsGameOver
                    ? ESlateVisibility::Visible
                    : ESlateVisibility::Collapsed
                );
            }
            // GameClearText 찾기
            if (UWidget* GameClearText = MainMenuWidgetInstance->GetWidgetFromName(TEXT("GameClearText")))
            {
                // Game Clear일 때만 표시
                GameClearText->SetVisibility(bIsGameClear
                    ? ESlateVisibility::Visible
                    : ESlateVisibility::Collapsed
                );
            }
            // Game Over 또는 Game Clear일 때 점수 표시
            if (bIsGameOver || bIsGameClear)
            {
                // TotalScoreText 찾기
                if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
                {
                    // GameInstance 가져오기
                    if (UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GetGameInstance()))
                    {
                        // 누적 점수를 Text에 미리 저장
                        TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("오늘 번 돈 : %d"), VODGameInstance->TotalScore)));
                        // 3초 뒤 TotalScoreAnim에서 보이게 함
                        TotalScoreText->SetRenderOpacity(0.0f);
                    }
                }
            }
            // Game Over 상태
            if (bIsGameOver)
            {
                // Blueprint의 PlayGameOverAnim 함수 찾기
                if (UFunction* PlayGameOverAnimFunction = MainMenuWidgetInstance->FindFunction(FName(TEXT("PlayGameOverAnim"))))
                {
                    // Game Over Animation 실행
                    MainMenuWidgetInstance->ProcessEvent(PlayGameOverAnimFunction, nullptr);
                }
            }
            // Game Clear 상태
            if (bIsGameClear)
            {
                // Blueprint의 PlayGameClearAnim 함수 찾기
                if (UFunction* PlayGameClearAnimFunction = MainMenuWidgetInstance->FindFunction(FName(TEXT("PlayGameClearAnim"))))
                {
                    // Game Clear 애니메이션 실행
                    MainMenuWidgetInstance->ProcessEvent(PlayGameClearAnimFunction, nullptr);
                }
            }
        }
    }
}
void AVODPlayerController::ShowGameHUD()
{
    // 기존 HUD 제거
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }
    // Main Menu 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    // HUD 생성
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            // HUD 표시
            HUDWidgetInstance->AddToViewport();
            // 마우스 숨김
            bShowMouseCursor = false;
            // 게임 입력 모드
            SetInputMode(FInputModeGameOnly());
            // 현재 GameState 가져오기
            AVODGameState* VODGameState = GetWorld()
                ? GetWorld()->GetGameState<
                AVODGameState
                >()
                : nullptr;
            // HUD 즉시 갱신
            if (VODGameState)
            {
                VODGameState->UpdateHUD();
            }
        }
    }
}
void AVODPlayerController::StartGame()
{
    // GameInstance 가져오기
    if (UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GetGameInstance()))
    {
        // 첫 Level로 초기화
        VODGameInstance->CurrentLevelIndex = 0;
        // 누적 점수 초기화
        VODGameInstance->TotalScore = 0;
    }
    // BasicLevel 열기
    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
    // Pause 해제
    SetPause(false);
}
void AVODPlayerController::ExitGame()
{
    // 게임 종료
    UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
void AVODPlayerController::ReturnToMainMenu()
{
    // Pause 해제
    SetPause(false);
    // UI 상태 초기화
    bIsUIMode = false;
    // MenuLevel 열기
    UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}
void AVODPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Enhanced Input으로 변환
    if (
        UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // UI 전환 입력 연결
        if (ToggleUIAction)
        {
            EnhancedInput->BindAction(ToggleUIAction, ETriggerEvent::Started, this, &AVODPlayerController:: ToggleMappingContext);
        }
    }
}
void AVODPlayerController::ToggleMappingContext(const FInputActionValue& Value)
{
    // Local Player 가져오기
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Enhanced Input Subsystem 가져오기
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem >())
        {
            // UI가 열려 있는 경우
            if (bIsUIMode)
            {
                // UI Mapping Context 제거
                if (UIInputMappingContext)
                {
                    Subsystem->RemoveMappingContext(UIInputMappingContext);
                }
                // 캐릭터 Mapping Context 등록
                if (InputMappingContext)
                {
                    Subsystem->AddMappingContext(InputMappingContext, 0);
                }
                // 게임 모드로 변경
                bIsUIMode = false;
                // 메뉴를 닫고 HUD 표시
                ShowGameHUD();
                UE_LOG(LogTemp, Warning, TEXT("게임 모드"));
            }
            else
            {
                // 캐릭터 Mapping Context 제거
                if (InputMappingContext)
                {
                    Subsystem->RemoveMappingContext(InputMappingContext);
                }
                // UI Mapping Context 등록
                if (UIInputMappingContext)
                {
                    Subsystem->AddMappingContext(UIInputMappingContext, 0);
                }
                // UI 모드로 변경
                bIsUIMode = true;
                // 메뉴 표시
                ShowMainMenu(true);
                // 게임과 UI 입력 모두 사용
                FInputModeGameAndUI InputMode;
                if (MainMenuWidgetInstance)
                {
                    InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
                }
                SetInputMode(InputMode);
                // 마우스 표시
                bShowMouseCursor = true;
            }
        }
    }
}