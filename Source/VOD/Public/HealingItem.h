#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class VOD_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// 생성자
	AHealingItem();
	// 회복할 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealAmount;
	// 회복 효과 실행
	virtual void ActivateItem(AActor* Activator) override;
};
