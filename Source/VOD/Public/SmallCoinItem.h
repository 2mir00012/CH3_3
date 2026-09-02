#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "SmallCoinItem.generated.h"

UCLASS()
class VOD_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	// 생성자
	ASmallCoinItem();
	// 작은 코인 획득 처리
	virtual void ActivateItem(AActor* Activator) override;
};