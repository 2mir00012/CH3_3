#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class TEST_1_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();
	// 플레이어가 회복할 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealAmount;
	// 힐링 아이템이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) override;
};
