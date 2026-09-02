#include "VODGameInstance.h"

UVODGameInstance::UVODGameInstance()
{
	// 누적 점수 초기화
	TotalScore = 0;
	// 현재 레벨 초기화
	CurrentLevelIndex = 0;
}
void UVODGameInstance::AddToScore(int32 Amount)
{
	// 누적 점수 증가
	TotalScore += Amount;
	// 현재 누적 점수 출력
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}