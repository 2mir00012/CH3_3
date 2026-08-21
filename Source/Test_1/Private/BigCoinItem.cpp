#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	// 큰 코인의 점수와 종류 설정
	PointValue = 50;
	ItemType = "BigCoin";
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	// DestroyItem 직접 호출에서 CoinItem의 공통 점수 획득 기능 호출로 변경
	Super::ActivateItem(Activator);
}

