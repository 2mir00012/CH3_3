#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class VOD_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	// 코인 획득 시 얻는 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	// 부모 클래스의 ActivateItem 오버라이드
	virtual void ActivateItem(AActor* Activator) override;
};
