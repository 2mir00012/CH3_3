#include "Item.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"

DEFINE_LOG_CATEGORY(LogTEST_1);

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 360.0f;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Resources/Props/SM_Chair.SM_Chair"));

	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));

	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0,MaterialAsset.Object);
	}

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(StaticMeshComp);

	UE_LOG(LogTEST_1, Warning, TEXT("%s Constructor"), *GetName());
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTEST_1, Warning, TEXT("%s PostInitializeComponents"), *GetName());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTEST_1, Warning, TEXT("%s BeginPlay"), *GetName());
	SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 45.0f, 0.0f));
	SetActorScale3D(FVector(2.0f));
	OnItemPickedUp();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}

void AItem::Destroyed()
{
	UE_LOG(LogTEST_1, Warning, TEXT("%s Destroyed"), *GetName());
	Super::Destroyed();
}

void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTEST_1, Warning, TEXT("%s EndPlay"), *GetName() );
	Super::EndPlay(EndPlayReason);
}

void AItem::ResetActorPosition()
{
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed() const
{
	return RotationSpeed;
}