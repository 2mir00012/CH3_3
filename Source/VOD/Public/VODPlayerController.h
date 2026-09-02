#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VODPlayerController.generated.h"

// 전방 선언
class UInputMappingContext;
class UInputAction;
class UUserWidget;

// Enhanced Input 값
struct FInputActionValue;

UCLASS()
class VOD_API AVODPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // 생성자
    AVODPlayerController();

    // =========================================
    // Input
    // =========================================
    // 캐릭터 조작용 Mapping Context
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;
    // UI 조작용 Mapping Context
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* UIInputMappingContext;
    // 이동 Input Action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;
    // 점프 Input Action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;
    // 카메라 회전 Input Action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;
    // 스프린트 Input Action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction;
    // UI 전환 Input Action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* ToggleUIAction;

    // =========================================
    // HUD
    // =========================================
    // HUD Widget 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;
    // 생성된 HUD Widget
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* HUDWidgetInstance;
    // HUD Widget 반환
    UFUNCTION(BlueprintPure, Category = "HUD")
    UUserWidget* GetHUDWidget() const;
    // HUD 표시
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameHUD();

    // =========================================
    // Menu
    // =========================================
    // Main Menu Widget 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
    // 생성된 Main Menu Widget
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
    UUserWidget* MainMenuWidgetInstance;
    // Main Menu 표시
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ShowMainMenu( bool bIsRestart, bool bIsGameOver = false, bool bIsGameClear = false);
    // 새로운 게임 시작 또는 재시작
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartGame();
    // 게임 종료
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ExitGame();
    // MenuLevel로 이동
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ReturnToMainMenu();

protected:
    // 게임 시작
    virtual void BeginPlay() override;
    // 입력 바인딩
    virtual void SetupInputComponent() override;
    // Tab으로 게임/UI 모드 전환
    void ToggleMappingContext(const FInputActionValue& Value);

private:
    // 현재 UI 모드 여부
    bool bIsUIMode;
};