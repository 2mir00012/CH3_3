#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "KeyItem.generated.h"

UCLASS()
class VOD_API AKeyItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// 플레이어와 Overlap했을 때 호출
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// 키 아이템 효과 실행
	virtual void ActivateItem(AActor* Activator) override;
};