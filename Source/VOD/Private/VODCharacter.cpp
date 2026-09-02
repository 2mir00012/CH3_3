#include "VODCharacter.h"
#include "VODPlayerController.h"
#include "VODGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AVODCharacter::AVODCharacter()
{
    // Tick 비활성화
    PrimaryActorTick.bCanEverTick = false;
    // AI 감지 대상 컴포넌트 생성
    StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
    // SpringArm 생성
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    // 카메라 거리
    SpringArmComp->TargetArmLength = 300.0f;
    // Controller 회전 사용
    SpringArmComp->bUsePawnControlRotation = true;
    // Camera 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // Camera 자체 회전 비활성화
    CameraComp->bUsePawnControlRotation = false;
    // 머리 위 HP Widget 생성
    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(GetMesh());
    // 화면 방향으로 UI 표시
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
    // 이동 속도 설정
    NormalSpeed = 250.0f;
    SprintSpeedMultiplier = 2.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    // 점프 횟수 초기화
    JumpCount = 0;
    // 마우스 감도
    MouseSensitivity = 1.0f;
    // 체력 초기화
    MaxHealth = 100.0f;
    Health = MaxHealth;
}
void AVODCharacter::BeginPlay()
{
    Super::BeginPlay();
    // 머리 위 HP 갱신
    UpdateOverheadHP();
    // AI Sight 감지 대상으로 등록
    if (StimuliSource)
    {
        StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
        StimuliSource->RegisterWithPerceptionSystem();
    }
}
int32 AVODCharacter::GetHealth() const
{
    // 현재 체력 반환
    return static_cast<int32>(Health);
}
float AVODCharacter::GetMaxHealth() const
{
    // 최대 체력 반환
    return MaxHealth;
}
void AVODCharacter::AddHealth(float Amount)
{
    // 체력 회복
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    // 머리 위 HP 갱신
    UpdateOverheadHP();
    UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), Health);
}
float AVODCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 부모 클래스의 데미지 처리
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    // 체력 감소
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    // 머리 위 HP 갱신
    UpdateOverheadHP();
    UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);
    // 체력이 0이면 사망
    if (Health <= 0.0f)
    {
        OnDeath();
    }
    return ActualDamage;
}
void AVODCharacter::OnDeath()
{
    // 사망 로그
    UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));
    // GameState 가져오기
    AVODGameState* VODGameState = GetWorld()
        ? GetWorld()->GetGameState<AVODGameState>()
        : nullptr;
    // Game Over 처리
    if (VODGameState)
    {
        VODGameState->OnGameOver();
    }
}
void AVODCharacter::UpdateOverheadHP()
{
    // WidgetComponent 확인
    if (!OverheadWidget)
    {
        return;
    }
    // 실제 Widget 가져오기
    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance)
    {
        return;
    }
    // HP Text 찾기
    if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
    {
        // 현재 체력 표시
        HPText->SetText(FText::FromString( FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
    }
}
void AVODCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Enhanced Input으로 변환
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // PlayerController 가져오기
        if (AVODPlayerController* PlayerController = Cast<AVODPlayerController>(GetController()))
        {
            // 이동 입력 연결
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AVODCharacter::Move);
            }
            // 점프 입력 연결
            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &AVODCharacter::StartJump);
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AVODCharacter::StopJump);
            }
            // 카메라 회전 입력 연결
            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AVODCharacter::Look);
            }
            // 스프린트 입력 연결
            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &AVODCharacter::StartSprint);
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &AVODCharacter::StopSprint);
            }
        }
    }
}
void AVODCharacter::Move(const FInputActionValue& Value)
{
    // Controller 확인
    if (!Controller)
    {
        return;
    }
    // 이동 입력값 가져오기
    const FVector2D MoveInput = Value.Get<FVector2D>();
    // 앞뒤 이동
    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }
    // 좌우 이동
    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}
void AVODCharacter::StartJump(const FInputActionValue& Value)
{
    // 점프 시작
    Jump();
    // 점프 횟수 증가
    JumpCount++;
    UE_LOG(LogTemp, Warning, TEXT("StartJump Called"));
    UE_LOG(LogTemp, Warning, TEXT("Jump Count: %d"), JumpCount);
}
void AVODCharacter::StopJump(const FInputActionValue& Value)
{
    // 점프 종료
    StopJumping();
    UE_LOG(LogTemp, Warning, TEXT("StopJump Called"));
}
void AVODCharacter::Look(const FInputActionValue& Value)
{
    // 마우스 입력값
    FVector2D LookInput = Value.Get<FVector2D>();
    // 좌우 회전
    AddControllerYawInput(LookInput.X * MouseSensitivity);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}
void AVODCharacter::StartSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        // 스프린트 속도 계산
        SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
        // 이동 속도 증가
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}
void AVODCharacter::StopSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        // 기본 이동 속도로 복귀
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}