#include "CoinItem.h"

ACoinItem::ACoinItem()
{
	// 코인의 기본 점수 설정
	PointValue = 0;
	// 기본 코인 타입 설정
	ItemType = "DefaultCoin";
}
// 코인 공통 획득 기능
void ACoinItem::ActivateItem(AActor* Activator)
{
	// 플레이어 태그 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 점수 획득 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));
		// 아이템 제거
		DestroyItem();
	}
}

