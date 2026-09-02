#include "TransformActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ATransformActor::ATransformActor()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;
	// 루트 컴포넌트 생성
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	// Mesh 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	// Coin Mesh 불러오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Props/SM_Coin_A.SM_Coin_A'"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
	// 이동 및 회전 기본값
	SpeedZ = 200.0f;
	RotationSpeed = 90.0f;
	// 이동 상태 초기화
	ElapsedTime = 0.0f;
	MoveDirection = 1.0f;
	StartZ = 0.0f;
	// 이동 범위
	MoveRange = 200.0f;
	// 크기 변화 주기
	ScalePeriod = 2.0f;
}
void ATransformActor::BeginPlay()
{
	Super::BeginPlay();
	// 초기 위치 설정
	SetActorLocation(FVector(0.0f, 0.0f, 100.0f));
	// 초기 회전 설정
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	// 초기 크기 설정
	SetActorScale3D(FVector(1.0f));
	// 시작 Z 위치 저장
	StartZ = GetActorLocation().Z;
	// Blueprint Event 실행
	OnTransformActorStarted();
}
void ATransformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Z축 이동 거리 계산
	float MoveAmount = SpeedZ * MoveDirection * DeltaTime;
	// 상하 이동
	AddActorWorldOffset(FVector(0.0f, 0.0f, MoveAmount));
	// 현재 높이
	float CurrentZ = GetActorLocation().Z;
	// 위쪽 범위 도달 시 방향 전환
	if (CurrentZ >= StartZ + MoveRange)
	{
		MoveDirection = -1.0f;
	}
	// 아래쪽 범위 도달 시 방향 전환
	else if (CurrentZ <= StartZ - MoveRange)
	{
		MoveDirection = 1.0f;
	}
	// Yaw 회전
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	// 시간 누적
	ElapsedTime += DeltaTime;
	// Sin 계산용 각도
	float ScaleAngle = (2.0f * PI / ScalePeriod) *ElapsedTime;
	// 크기 변화 값 계산
	float ScaleValue = 1.0f + 0.5f * FMath::Sin(ScaleAngle);
	// Scale 적용
	SetActorScale3D(FVector(ScaleValue));
}
void ATransformActor::ResetPosition()
{
	// 현재 위치 가져오기
	FVector CurrentLocation = GetActorLocation();
	// Z축을 시작 위치로 초기화
	CurrentLocation.Z = StartZ;
	SetActorLocation(CurrentLocation);
}
float ATransformActor::GetRotationSpeed() const
{
	// 회전 속도 반환
	return RotationSpeed;
}