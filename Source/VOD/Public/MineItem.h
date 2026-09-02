#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

// 전방 선언
class USphereComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class VOD_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// 생성자
	AMineItem();

protected:
	// 폭발 범위 충돌 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;
	// 폭발 파티클
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	// 폭발 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;
	// 폭발까지의 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDelay;
	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius;
	// 폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	int ExplosionDamage;
	// 중복 폭발 방지
	bool bHasExploded;
	// 폭발 Timer
	FTimerHandle ExplosionTimerHandle;
	// 지뢰 활성화
	virtual void ActivateItem(AActor* Activator) override;
	// 실제 폭발 처리
	void Explode();
};
