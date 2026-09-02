#include "HealingItem.h"
#include "VODCharacter.h"

AHealingItem::AHealingItem()
{
	// 체력 회복량
	HealAmount = 20.0f;
	// 아이템 종류
	ItemType = "Healing";
}
void AHealingItem::ActivateItem(AActor* Activator)
{
	// 공통 획득 효과 실행
	Super::ActivateItem(Activator);
	// Player가 획득했는지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// Player Character로 변환
		if (AVODCharacter* PlayerCharacter = Cast<AVODCharacter>(Activator))
		{
			// 체력 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
		// 아이템 제거
		DestroyItem();
	}
}

