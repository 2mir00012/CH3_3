#include "Item.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

// 로그 카테고리 정의
DEFINE_LOG_CATEGORY(LogVOD);

AItem::AItem()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;
	// 기본 회전 속도
	RotationSpeed = 360.0f;
	// 루트 컴포넌트 생성
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	// Mesh 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	// Chair Mesh 불러오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Game/Resources/Props/SM_Chair.SM_Chair"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
	// Material 불러오기
	static ConstructorHelpers::FObjectFinder<UMaterial>
		MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset. Object);
	}
	// Audio 컴포넌트 생성
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(StaticMeshComp);
	// 생성자 호출 확인
	UE_LOG(LogVOD, Warning, TEXT("%s Constructor"), *GetName());
}
void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Component 초기화 확인
	UE_LOG(LogVOD, Warning, TEXT("%s PostInitializeComponents"), *GetName());
}
void AItem::BeginPlay()
{
	Super::BeginPlay();
	// BeginPlay 호출 확인
	UE_LOG(LogVOD, Warning, TEXT("%s BeginPlay"), *GetName());
	// 초기 위치 변경
	SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
	// 초기 회전 변경
	SetActorRotation(FRotator(0.0f, 45.0f, 0.0f));
	// 초기 크기 변경
	SetActorScale3D(FVector(2.0f));
	// Blueprint Event 실행
	OnItemPickedUp();
}
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 회전 속도가 0이 아니면 회전
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(
			FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}
void AItem::Destroyed()
{
	// Destroyed 호출 확인
	UE_LOG(LogVOD, Warning, TEXT("%s Destroyed"), *GetName());
	Super::Destroyed();
}
void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// EndPlay 호출 확인
	UE_LOG(LogVOD, Warning,TEXT("%s EndPlay"), *GetName());
	Super::EndPlay(EndPlayReason);
}
void AItem::ResetActorPosition()
{
	// 위치를 원점으로 초기화
	SetActorLocation(FVector::ZeroVector);
}
float AItem::GetRotationSpeed() const
{
	// 회전 속도 반환
	return RotationSpeed;
}