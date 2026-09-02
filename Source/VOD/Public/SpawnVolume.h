#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class VOD_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	ASpawnVolume();
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	// 아이템 생성 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	// 아이템 스폰 정보를 가진 DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;
	// 최대 생성 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxSpawnCount;
	// 현재 Wave의 생성 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 CurrentSpawnCount;
	// 아이템 주변에 확보할 빈 공간 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Collision")
	float SpawnCheckRadius;
	// 바닥에서 아이템을 띄울 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Collision")
	float SpawnHeightOffset;
	// 안전한 위치를 찾을 최대 시도 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Collision")
	int32 MaxSpawnAttempts;
	// 랜덤 아이템 생성
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();
	// Wave 시작 시 생성 개수 초기화
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void ResetSpawnCount();
	// 확률에 따라 DataTable Row 선택
	FItemSpawnRow* GetRandomItem() const;
	// SpawnVolume 내부 랜덤 위치 반환
	FVector GetRandomPointInVolume() const;
	// 오브젝트와 겹치지 않는 위치 찾기
	bool FindValidSpawnLocation(FVector& OutLocation) const;
	// 아이템 Actor 생성
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
};
