#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	// 작은 코인의 점수와 종류 설정
	PointValue = 10;
	ItemType = "SmallCoin";
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	// DestroyItem 직접 호출에서 CoinItem의 공통 점수 획득 기능 호출로 변경
	Super::ActivateItem(Activator);
}

