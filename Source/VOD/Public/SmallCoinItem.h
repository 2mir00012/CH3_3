#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "SmallCoinItem.generated.h"

UCLASS()
class VOD_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	ASmallCoinItem();
	// 작은 코인이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) override;
};
