#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VODGameInstance.generated.h"

UCLASS()
class VOD_API UVODGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 생성자
	UVODGameInstance();
	// 게임 전체 누적 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 현재 레벨 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
	// 누적 점수 증가
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};