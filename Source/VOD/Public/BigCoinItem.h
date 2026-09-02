#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class VOD_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	// 생성자
	ABigCoinItem();
	// 큰 코인 획득 처리
	virtual void ActivateItem(AActor* Activator) override;
};
