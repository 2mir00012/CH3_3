#include "HealingItem.h"

AHealingItem::AHealingItem()
{
	// 회복량과 아이템 종류 설정
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	// 플레이어 태그 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		//] 회복 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP!"), HealAmount));
		// 사용된 힐링 아이템 제거
		DestroyItem();
	}
}

