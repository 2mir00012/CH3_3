#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class TEST_1_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();

protected:
	// 폭발 범위를 확인하는 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;
	// 폭발까지 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDelay;
	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius;
	// 폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	int ExplosionDamage;
	// 폭발 타이머 관리
	FTimerHandle ExplosionTimerHandle;
	// 지뢰가 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) override;
	// 실제 폭발 처리 함수
	void Explode();
};
