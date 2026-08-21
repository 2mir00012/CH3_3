#include "TransformActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ATransformActor::ATransformActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Props/SM_Coin_A.SM_Coin_A'"));

	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	SpeedZ = 200.0f;
	RotationSpeed = 90.0f;
	ElapsedTime = 0.0f;
	MoveDirection = 1.0f;
	StartZ = 0.0f;
	MoveRange = 200.0f;
	ScalePeriod = 2.0f;
}

void ATransformActor::BeginPlay()

{
	Super::BeginPlay();
	SetActorLocation(FVector(0.0f, 0.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	SetActorScale3D(FVector(1.0f));
	StartZ = GetActorLocation().Z;
	OnTransformActorStarted();
}

void ATransformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float MoveAmount = SpeedZ * MoveDirection * DeltaTime;
	AddActorWorldOffset(FVector(0.0f, 0.0f, MoveAmount));
	float CurrentZ = GetActorLocation().Z;

	if (CurrentZ >= StartZ + MoveRange)
	{
		MoveDirection = -1.0f;
	}
	else if (CurrentZ <= StartZ - MoveRange)
	{
		MoveDirection = 1.0f;
	}

	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	ElapsedTime += DeltaTime;
	float ScaleAngle = (2.0f * PI / ScalePeriod) * ElapsedTime;
	float ScaleValue = 1.0f + 0.5f * FMath::Sin(ScaleAngle);
	SetActorScale3D(FVector(ScaleValue));
}

void ATransformActor::ResetPosition()
{
	FVector CurrentLocation = GetActorLocation();

	CurrentLocation.Z = StartZ;

	SetActorLocation(CurrentLocation);
}

float ATransformActor::GetRotationSpeed() const
{
	return RotationSpeed;
}