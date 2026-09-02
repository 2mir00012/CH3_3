#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

ASpawnVolume::ASpawnVolume()
{
	// Tick 비활성화
	PrimaryActorTick.bCanEverTick = false;
	// 루트 컴포넌트 생성
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	// 아이템 생성 범위 생성
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
	// DataTable 초기화
	ItemDataTable = nullptr;
	// 스폰 개수 초기화
	MaxSpawnCount = 40;
	CurrentSpawnCount = 0;
	// 주변 오브젝트와 이 정도 거리 확보
	SpawnCheckRadius = 60.0f;
	// 바닥보다 살짝 위에서 생성
	SpawnHeightOffset = 30.0f;
	// 최대 30번까지 빈 공간 검색
	MaxSpawnAttempts = 30;
}
AActor* ASpawnVolume::SpawnRandomItem()
{
	// 확률에 따라 아이템 Row 선택
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		// 선택한 아이템 클래스 가져오기
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			// 아이템 생성
			AActor* SpawnedActor = SpawnItem(ActualClass);
			// 생성 성공 시 개수 증가
			if (SpawnedActor)
			{
				CurrentSpawnCount++;
				UE_LOG(LogTemp, Verbose, TEXT("Spawn Success: %d / %d"), CurrentSpawnCount, MaxSpawnCount);
			}
			// 생성된 Actor 반환
			return SpawnedActor;
		}
	}
	// 생성 실패
	return nullptr;
}
void ASpawnVolume::ResetSpawnCount()
{
	// 현재 스폰 개수 확인
	UE_LOG(LogTemp, Warning, TEXT("Spawn Count Reset: %d -> 0"), CurrentSpawnCount);
	// Wave별 스폰 개수 초기화
	CurrentSpawnCount = 0;
}
FVector ASpawnVolume::GetRandomPointInVolume() const
{
	// 박스 절반 크기
	const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// 박스 중심 위치
	const FVector BoxOrigin = SpawningBox->GetComponentLocation();
	// 박스 내부 랜덤 위치 반환
	return BoxOrigin + FVector(FMath::FRandRange(-BoxExtent.X, BoxExtent.X), FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}
bool ASpawnVolume::FindValidSpawnLocation(FVector& OutLocation) const
{
	if (!GetWorld() || !SpawningBox)
	{
		return false;
	}
	const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	const FVector BoxOrigin = SpawningBox->GetComponentLocation();
	// 최대 횟수만큼 새로운 위치 검색
	for (
		int32 Attempt = 0;
		Attempt < MaxSpawnAttempts;
		Attempt++
		)
	{
		// X / Y 랜덤 위치 선택
		const float RandomX = FMath::FRandRange(-BoxExtent.X, BoxExtent.X);
		const float RandomY = FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y);
		// 위에서 아래로 Line Trace
		const FVector TraceStart(BoxOrigin.X + RandomX, BoxOrigin.Y + RandomY, BoxOrigin.Z + BoxExtent.Z);
		const FVector TraceEnd(BoxOrigin.X + RandomX, BoxOrigin.Y + RandomY, BoxOrigin.Z - BoxExtent.Z);
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		// SpawnVolume 자기 자신 제외
		TraceParams.AddIgnoredActor(this);
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
		// 아무것도 맞지 않았다면 다른 위치 검색
		if (!bHit)
		{
			continue;
		}
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor)
		{
			continue;
		}
		// SpawnFloor인지 확인
		bool bIsSpawnFloor = HitActor->ActorHasTag(TEXT("SpawnFloor"));
		// Actor Tag 대신 Component Tag를 사용하는 경우도 허용
		if (!bIsSpawnFloor && HitResult.GetComponent())
		{
			bIsSpawnFloor = HitResult
				.GetComponent()->ComponentHasTag(TEXT("SpawnFloor"));
		}
		// 책상, 선반, 세탁기 등을 맞았다면 해당 위치 사용하지 않음
		if (!bIsSpawnFloor)
		{
			continue;
		}
		// 실제 Spawn 후보 위치
		const FVector CandidateLocation = HitResult.ImpactPoint + FVector( 0.0f, 0.0f, SpawnHeightOffset);
		// 주변 오브젝트 검사
		const FCollisionShape CheckShape = FCollisionShape::MakeSphere(SpawnCheckRadius);
		FCollisionQueryParams OverlapParams;
		OverlapParams.AddIgnoredActor(this);
		// 검사할 Object Type
		FCollisionObjectQueryParams ObjectParams;
		// 벽, 바닥, 책상, 선반 등
		ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		// 움직일 수 있는 오브젝트
		ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		// 플레이어 위치에도 Spawn되지 않게 함
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
		const bool bBlocked = GetWorld()->OverlapAnyTestByObjectType(CandidateLocation, FQuat::Identity, ObjectParams, CheckShape, OverlapParams);
		// 주변에 오브젝트가 있다면 다른 위치 검색 
		if (bBlocked)
		{
			continue;
		}
		// 사용 가능한 위치 발견
		OutLocation = CandidateLocation;
		return true;
	}
	// 최대 시도 횟수 동안 빈 공간을 찾지 못함
	return false;
}
FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	// DataTable 확인
	if (!ItemDataTable)
	{
		return nullptr;
	}
	// 모든 Row 가져오기
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);
	// Row가 없으면 종료
	if (AllRows.IsEmpty())
	{
		return nullptr;
	}
	// 전체 스폰 확률 계산
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	// 확률 범위에서 랜덤 값 생성
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	// 누적 확률로 아이템 선택
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}
AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	// ItemClass 확인
	if (!ItemClass)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnItem: ItemClass is NULL!") );
		return nullptr;
	}
	// World 확인
	if (!GetWorld())
	{
		return nullptr;
	}
	// 안전한 위치 먼저 찾기
	FVector SpawnLocation;
	if (!FindValidSpawnLocation(SpawnLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnItem: Valid Spawn Location Not Found"));
		return nullptr;
	}
	// Spawn 설정
	FActorSpawnParameters SpawnParams;
	// 오브젝트와 충돌할 경우 억지로 생성하지 않음
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	// Actor 생성
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnItem Failed!"));
	}
	return SpawnedActor;
}