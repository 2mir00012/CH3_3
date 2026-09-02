#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	// 큰 코인 점수
	PointValue = 50;
	// 아이템 종류
	ItemType = "BigCoin";
}
void ABigCoinItem::ActivateItem(AActor* Activator)
{
	// CoinItem의 공통 획득 처리 실행
	Super::ActivateItem(Activator);
}

