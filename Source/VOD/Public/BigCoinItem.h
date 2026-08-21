#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class VOD_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	ABigCoinItem();
	// 큰 코인이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) override;
};
