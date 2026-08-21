#include "VODCharacter.h"
// PlayerController
#include "VODPlayerController.h"
// Enhanced Input
#include "EnhancedInputComponent.h"
// Camera
#include "Camera/CameraComponent.h"
// SpringArm
#include "GameFramework/SpringArmComponent.h"
// CharacterMovementComponent
#include "GameFramework/CharacterMovementComponent.h"


AVODCharacter::AVODCharacter()
{
    // Tick을 사용하지 않으므로 비활성화
    PrimaryActorTick.bCanEverTick = false;
    // SpringArm 생성
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // Character의 RootComponent에 부착
    SpringArmComp->SetupAttachment(RootComponent);
    // 캐릭터와 카메라 사이의 거리
    SpringArmComp->TargetArmLength = 300.0f;
    // Controller의 회전을 SpringArm이 따라가도록 설정
    SpringArmComp->bUsePawnControlRotation = true;
    // Camera 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // SpringArm 끝에 Camera 부착
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // Camera 자체는 Controller Rotation을 직접 사용하지 않음
    CameraComp->bUsePawnControlRotation = false;
    // 이동 속도 설정
    // 기본 이동 속도
    NormalSpeed = 250.0f;
    // 스프린트 배율
    SprintSpeedMultiplier = 2.0f;
    // 600 * 1.5 = 900
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    // 시작할 때 기본 이동 속도 적용
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    //점프 횟수 초기화
    JumpCount = 0;
    //마우스 감도 기본값
    MouseSensitivity = 1.0f;
}

// Input Action 연결
void AVODCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // 부모 클래스의 입력 설정 실행
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // UInputComponent를 Enhanced Input용으로 Cast
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 현재 Controller를 우리가 만든 PlayerController로 Cast
        if (AVODPlayerController* PlayerController = Cast<AVODPlayerController>(GetController()))
        {
            // Move
            if (PlayerController->MoveAction)
            {
                // 이동 입력 중 Move() 실행
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AVODCharacter::Move);
            }
            // Jump
            if (PlayerController->JumpAction)
            {
                // 점프 키를 누르는 동안
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this,&AVODCharacter::StartJump);
                // 점프 키를 뗐을 때
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AVODCharacter::StopJump);
            }
            // Look
            if (PlayerController->LookAction)
            {
                // 마우스가 움직이는 동안 Look() 실행
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AVODCharacter::Look);
            }
            // Sprint
            if (PlayerController->SprintAction)
            {
                // Shift 입력 중
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &AVODCharacter::StartSprint);
                // Shift 입력 종료
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &AVODCharacter::StopSprint);
            }
        }
    }
}
// Move
void AVODCharacter::Move(const FInputActionValue& Value)
{
    // Controller가 없다면 이동 처리 불가
    if (!Controller)
    {
        return;
    }
    // IA_Move의 Axis2D 입력값 가져오기
    const FVector2D MoveInput = Value.Get<FVector2D>();
    // X축 입력이 있다면 앞뒤 이동
    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 캐릭터의 앞 방향으로 이동
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }
    // Y축 입력이 있다면 좌우 이동
    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // 캐릭터의 오른쪽 방향으로 이동
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}
// Jump
void AVODCharacter::StartJump(const FInputActionValue& Value)
{
    // ACharacter의 기본 Jump 함수 실행
    Jump();
    // 점프 횟수 증가
    JumpCount++;
    // StartJump 호출 확인
    UE_LOG(LogTemp, Warning, TEXT("StartJump Called"));
    // 현재 점프 횟수 출력
    UE_LOG(LogTemp, Warning, TEXT("Jump Count: %d"), JumpCount);
}
void AVODCharacter::StopJump(const FInputActionValue& Value)
{
    // 점프 입력 종료
    StopJumping();
    // StopJump 호출 확인
    UE_LOG(LogTemp, Warning, TEXT("StopJump Called"));
}
// Look
void AVODCharacter::Look(const FInputActionValue& Value)
{
    // 마우스 X, Y 입력값 가져오기
    FVector2D LookInput = Value.Get<FVector2D>();
    // 좌우 회전 + 마우스 감도 적용
    AddControllerYawInput(LookInput.X * MouseSensitivity);
    // 상하 회전 + 마우스 감도 적용
    AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}
// Sprint
void AVODCharacter::StartSprint(const FInputActionValue& Value)
{
    // CharacterMovementComponent 확인
    if (GetCharacterMovement())
    {
        // 현재 설정한 배율을 이용해 스프린트 속도 계산
        SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
        // 스프린트 속도 적용
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}
void AVODCharacter::StopSprint(const FInputActionValue& Value)
{
    // CharacterMovementComponent 확인
    if (GetCharacterMovement())
    {
        // 최대 이동 속도를 기본 속도로 복구
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}