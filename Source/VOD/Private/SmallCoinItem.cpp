#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	// 작은 코인 점수
	PointValue = 10;
	// 아이템 종류
	ItemType = "SmallCoin";
}
void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	// CoinItem의 공통 획득 처리 실행
	Super::ActivateItem(Activator);
}
