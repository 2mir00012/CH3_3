#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "KeyItem.generated.h"

UCLASS()
class TEST_1_API AKeyItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// 키 아이템과 오버랩했을 때 호출
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// 키 아이템을 사용했을 때 호출
	virtual void ActivateItem(AActor* Activator) override;
};