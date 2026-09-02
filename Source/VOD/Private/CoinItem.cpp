#include "CoinItem.h"
#include "Engine/World.h"
#include "VODGameState.h"

ACoinItem::ACoinItem()
{
	// 기본 점수
	PointValue = 0;
	// 기본 코인 종류
	ItemType = "DefaultCoin";
}
void ACoinItem::ActivateItem(AActor* Activator)
{
	// 공통 획득 효과 실행
	Super::ActivateItem(Activator);
	// Player가 획득했는지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			// 현재 GameState 가져오기
			if (AVODGameState* GameState = World->GetGameState<AVODGameState>())
			{
				// 점수 증가
				GameState->AddScore(PointValue);
				// 코인 획득 수 증가
				GameState->OnCoinCollected();
			}
		}
		// 코인 제거
		DestroyItem();
	}
}

