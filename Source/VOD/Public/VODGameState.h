#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "VODGameState.generated.h"

UCLASS()
class VOD_API AVODGameState : public AGameState
{
	GENERATED_BODY()

public:
	// 생성자
	AVODGameState();
	// 게임 시작
	virtual void BeginPlay() override;

	// =========================================
	// Score
	// =========================================
	// 현재 레벨 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	// 레벨 클리어 목표 점수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	int32 TargetScore;
	// 현재 점수 반환
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	// 점수 증가
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	// =========================================
	// Coin
	// =========================================
	// 현재 Wave에서 생성된 코인 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	// 현재 Wave에서 획득한 코인 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	// =========================================
	// Mine
	// =========================================
	// 폭발한 지뢰 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
	int32 ExplodedMineCount;

	// =========================================
	// Level
	// =========================================
	// 현재 Wave 제한 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	// 현재 레벨 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	// 전체 레벨 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	// 이동할 레벨 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	// =========================================
	// Wave
	// =========================================
	// 현재 Wave
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWave;
	// 전체 Wave 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	// Wave별 제한 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<float> WaveDurations;
	// Wave별 아이템 생성 횟수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<int32> WaveItemCounts;
	// 현재 Wave에서 생성된 Actor
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedWaveItems;

	// =========================================
	// Timer
	// =========================================
	// Wave 제한 시간 Timer
	FTimerHandle LevelTimerHandle;
	// HUD 갱신 Timer
	FTimerHandle HUDUpdateTimerHandle;

	// =========================================
	// Game Flow
	// =========================================
	// 현재 레벨 시작
	void StartLevel();
	// 현재 Wave 시작
	void StartWave();
	// 현재 Wave 종료
	void EndWave();
	// 제한 시간 종료 처리
	void OnLevelTimeUp();
	// 코인 획득 처리
	void OnCoinCollected();
	// 지뢰 폭발 처리
	void OnMineExploded();
	// 현재 레벨 종료
	void EndLevel();
	// 게임 오버 처리
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	// 게임 클리어 처리
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameClear();

	// =========================================
	// Wave Getter
	// =========================================
	// 현재 Wave 반환
	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetCurrentWave() const;
	// 전체 Wave 수 반환
	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetMaxWaves() const;

	// =========================================
	// HUD
	// =========================================
	// HUD 정보 갱신
	void UpdateHUD();

private:
	// Wave 중복 종료 방지
	bool bWaveEnding;
	// 이전 Wave 아이템 제거
	void ClearWaveItems();
	// 현재 Wave 제한 시간 반환
	float GetCurrentWaveDuration() const;
	// 현재 Wave 스폰 횟수 반환
	int32 GetCurrentWaveItemCount() const;
};