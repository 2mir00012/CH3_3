#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class VOD_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// 생성자
	ACoinItem();

protected:
	// 획득 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	// 코인 획득 처리
	virtual void ActivateItem(AActor* Activator) override;
};
