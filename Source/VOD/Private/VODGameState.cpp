#include "VODGameState.h"
#include "VODGameInstance.h"
#include "VODPlayerController.h"
#include "VODCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

AVODGameState::AVODGameState()
{
	// 점수 초기화
	Score = 0;
	TargetScore = 100;
	// 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	// 폭발한 지뢰 수 초기화
	ExplodedMineCount = 0;
	// 레벨 정보 초기화
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	// Wave 정보 초기화
	CurrentWave = 1;
	MaxWaves = 3;
	// Wave별 제한 시간
	WaveDurations =
	{
		30.0f,
		25.0f,
		20.0f
	};
	// Wave별 아이템 생성 횟수
	WaveItemCounts =
	{
		20,
		30,
		40
	};
	// Wave 종료 상태 초기화
	bWaveEnding = false;
}
void AVODGameState::BeginPlay()
{
	Super::BeginPlay();
	// 현재 레벨 시작
	StartLevel();
	// HUD 주기적 갱신
	GetWorldTimerManager()
		.SetTimer(HUDUpdateTimerHandle, this, &AVODGameState::UpdateHUD, 0.1f, true);
}
int32 AVODGameState::GetScore() const
{
	// 현재 점수 반환
	return Score;
}
void AVODGameState::AddScore(int32 Amount)
{
	// 점수 증가 전 값 저장
	const int32 PreviousScore = Score;
	// 현재 레벨 점수 증가
	Score += Amount;
	// 전체 누적 점수 증가
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GameInstance);
		if (VODGameInstance)
		{
			VODGameInstance->AddToScore(Amount);
		}
	}
	// 50점 도달 확인
	if (PreviousScore < 50 && Score >= 50)
	{
		UE_LOG(LogTemp, Warning, TEXT("50점 도달!!!"));
	}
	// 100점 도달 확인
	if (PreviousScore < 100 && Score >= 100)
	{
		UE_LOG(LogTemp, Warning, TEXT("보너스 체력 20을 획득!!!"));
	}
}
void AVODGameState::StartLevel()
{
	// 게임 HUD 표시
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AVODPlayerController* VODPlayerController = Cast<AVODPlayerController>(PlayerController))
		{
			VODPlayerController->ShowGameHUD();
		}
	}
	// GameInstance에서 현재 레벨 가져오기
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GameInstance))
		{
			CurrentLevelIndex = VODGameInstance->CurrentLevelIndex;
		}
	}
	// 첫 Wave로 초기화
	CurrentWave = 1;
	// 배열 기준 전체 Wave 수 계산
	MaxWaves = FMath::Min(WaveDurations.Num(), WaveItemCounts.Num());
	// 최소 한 개의 Wave 보장
	if (MaxWaves <= 0)
	{
		MaxWaves = 1;
	}
	// 레벨 시작 로그
	UE_LOG(LogTemp, Warning, TEXT("============================"));
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!"), CurrentLevelIndex + 1);
	UE_LOG(LogTemp, Warning, TEXT("============================"));
	// 첫 Wave 시작
	StartWave();
}
void AVODGameState::StartWave()
{
	// 이전 Wave 아이템 제거
	ClearWaveItems();
	// Wave 종료 상태 초기화
	bWaveEnding = false;
	// 코인 수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	// 현재 Wave 설정 가져오기
	LevelDuration = GetCurrentWaveDuration();
	const int32 ItemToSpawn = GetCurrentWaveItemCount();
	// SpawnVolume 검색
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	// 실제 생성된 아이템 수
	int32 SuccessfulSpawnCount = 0;
	// SpawnVolume이 존재하면 아이템 생성
	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{
			// Wave별 Spawn Count 초기화
			SpawnVolume->ResetSpawnCount();
			// Wave 설정 개수만큼 생성
			for (int32 i = 0;
				i < ItemToSpawn;
				i++
				)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor)
				{
					// 실제 생성 개수 증가
					SuccessfulSpawnCount++;
					// 생성된 Actor 저장
					SpawnedWaveItems.Add(SpawnedActor);
					// Coin이면 코인 개수 증가
					if (SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
				}
			}
		}
	}
	else
	{
		// SpawnVolume 검색 실패
		UE_LOG(LogTemp, Error, TEXT("SpawnVolume Not Found!"));
	}
	// Wave 제한 시간 시작
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &AVODGameState::OnLevelTimeUp, LevelDuration, false);
	// Wave 시작 로그
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), CurrentWave);
	// Spawn 결과 로그
	UE_LOG(LogTemp, Warning, TEXT("Wave %d | Time: %.1f | Request: %d | Success: %d | Coins: %d"), CurrentWave, LevelDuration, ItemToSpawn, SuccessfulSpawnCount, SpawnedCoinCount);
	// 화면에 Wave 시작 표시
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage( -1, 3.0f, FColor::Yellow, FString::Printf( TEXT("Wave %d Start!"), CurrentWave));
	}
	// HUD 즉시 갱신
	UpdateHUD();
}
void AVODGameState::OnLevelTimeUp()
{
	// 제한 시간 종료 로그
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Time Up!"), CurrentWave );
	// 현재 Wave 종료
	EndWave();
}
void AVODGameState::OnCoinCollected()
{
	// Wave 종료 중이면 추가 처리하지 않음
	if (bWaveEnding)
	{
		return;
	}
	// 획득 코인 수 증가
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Coin Collected: %d / %d"), CurrentWave, CollectedCoinCount, SpawnedCoinCount);
	// 모든 Coin 획득 시 Wave 종료
	if (
		SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}
void AVODGameState::EndWave()
{
	// 중복 종료 방지
	if (bWaveEnding)
	{
		return;
	}
	bWaveEnding = true;
	// Wave Timer 종료
	GetWorldTimerManager()
		.ClearTimer(LevelTimerHandle);
	// Wave 종료 로그
	UE_LOG(LogTemp, Warning, TEXT("Wave %d End!"), CurrentWave);
	// 다음 Wave가 남아 있는 경우
	if (CurrentWave < MaxWaves)
	{
		// Wave 증가
		CurrentWave++;
		UE_LOG(LogTemp, Warning, TEXT("Next Wave: %d"), CurrentWave);
		// 다음 Tick에서 새 Wave 시작
		GetWorldTimerManager()
			.SetTimerForNextTick(this, &AVODGameState::StartWave);
		return;
	}
	// 전체 Wave 완료
	UE_LOG(LogTemp, Warning, TEXT("All Waves Complete!"));
	// 목표 점수 달성 확인
	if (Score >= TargetScore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Score Cleared: %d / %d"), Score, TargetScore);
		UE_LOG(LogTemp, Warning, TEXT("Level %d Clear!"), CurrentLevelIndex + 1);
		// 다음 Level 진행
		GetWorldTimerManager()
			.SetTimerForNextTick(this, &AVODGameState::EndLevel);
	}
	else
	{
		// 목표 점수 실패
		UE_LOG(LogTemp, Warning, TEXT("Target Score Failed: %d / %d"), Score, TargetScore);
		// Game Over
		GetWorldTimerManager()
			.SetTimerForNextTick(this, &AVODGameState::OnGameOver);
	}
}
void AVODGameState::ClearWaveItems()
{
	// 현재 Wave에서 생성된 Actor 제거
	for (AActor* SpawnedActor : SpawnedWaveItems)
	{
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->Destroy();
		}
	}
	// 배열 비우기
	SpawnedWaveItems.Empty();
}
void AVODGameState::OnMineExploded()
{
	// 폭발한 지뢰 수 증가
	ExplodedMineCount++;
	// 폭발 횟수 로그
	UE_LOG(LogTemp, Warning, TEXT("Mine Exploded: %d"), ExplodedMineCount);
}
void AVODGameState::EndLevel()
{
	// Wave Timer 종료
	GetWorldTimerManager()
		.ClearTimer(LevelTimerHandle);
	// 남아 있는 아이템 제거
	ClearWaveItems();
	// GameInstance 가져오기
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GameInstance);
		if (VODGameInstance)
		{
			// 다음 Level Index로 변경
			CurrentLevelIndex++;
			// GameInstance에 저장
			VODGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			// 마지막 Level까지 완료
			if (CurrentLevelIndex >= MaxLevels)
			{
				// 모든 Level 완료
				OnGameClear();
				return;
			}
			// 다음 Level 열기
			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				// 다음 Level 정보가 없으면 종료
				OnGameOver();
			}
		}
	}
}
void AVODGameState::OnGameOver()
{
	// Wave Timer 종료
	GetWorldTimerManager()
		.ClearTimer(LevelTimerHandle);
	// PlayerController 가져오기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AVODPlayerController* VODPlayerController = Cast<AVODPlayerController>(PlayerController))
		{
			// 게임 일시정지
			VODPlayerController->SetPause(true);
			// Game Over 메뉴 표시
			VODPlayerController->ShowMainMenu(true, true);
		}
	}
	// Game Over 로그
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}
void AVODGameState::OnGameClear()
{
	// Wave Timer 종료
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 남은 Wave 아이템 제거
	ClearWaveItems();
	// PlayerController 가져오기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AVODPlayerController* VODPlayerController = Cast<AVODPlayerController>(PlayerController))
		{
			// 게임 일시정지
			VODPlayerController->SetPause(true);
			// Game Clear 메뉴 표시
			VODPlayerController->ShowMainMenu(true, false, true);
		}
	}
	// Game Clear 로그
	UE_LOG(LogTemp, Warning, TEXT("GAME CLEAR!!"));
}
int32 AVODGameState::GetCurrentWave() const
{
	// 현재 Wave 반환
	return CurrentWave;
}
int32 AVODGameState::GetMaxWaves() const
{
	// 전체 Wave 수 반환
	return MaxWaves;
}
float AVODGameState::GetCurrentWaveDuration() const
{
	// Wave 번호를 배열 Index로 변환
	const int32 WaveIndex = CurrentWave - 1;
	// 현재 Wave 시간 확인
	if (WaveDurations
		.IsValidIndex(WaveIndex))
	{
		const float Duration = WaveDurations[WaveIndex];
		if (Duration > 0.0f)
		{
			return Duration;
		}
		UE_LOG(LogTemp, Error, TEXT("Wave %d Duration is invalid: %.1f"), CurrentWave, Duration);
	}
	// 기본 제한 시간
	return 30.0f;
}
int32 AVODGameState::GetCurrentWaveItemCount() const
{
	// Wave 번호를 배열 Index로 변환
	const int32 WaveIndex = CurrentWave - 1;
	// 현재 Wave 스폰 수 확인
	if (WaveItemCounts
		.IsValidIndex(WaveIndex))
	{
		const int32 ItemCount = WaveItemCounts[WaveIndex];
		if (ItemCount > 0)
		{
			return ItemCount;
		}
		UE_LOG(LogTemp, Error, TEXT("Wave %d ItemCount is invalid: %d"), CurrentWave, ItemCount);
	}
	// 기본 생성 횟수
	return 40;
}
void AVODGameState::UpdateHUD()
{
	// PlayerController 가져오기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AVODPlayerController* VODPlayerController = Cast<AVODPlayerController>(PlayerController);
		if (!VODPlayerController)
		{
			return;
		}
		// HUD Widget 가져오기
		UUserWidget* HUDWidget = VODPlayerController->GetHUDWidget();
		if (!HUDWidget)
		{
			return;
		}
		// 남은 시간 표시
		if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
		{
			float RemainingTime = GetWorldTimerManager()
				.GetTimerRemaining(LevelTimerHandle);
			// 음수 방지
			RemainingTime = FMath::Max(0.0f, RemainingTime);
			TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
		}
		// 누적 점수 표시
		if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
		{
			if (UGameInstance* GameInstance = GetGameInstance())
			{
				if (UVODGameInstance* VODGameInstance = Cast<UVODGameInstance>(GameInstance))
				{
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("오늘 번 돈 : %d"), VODGameInstance->TotalScore)));
				}
			}
		}
		
        // Level - 현재 알바 장소 표시
		if (UTextBlock* LevelText = Cast<UTextBlock>( HUDWidget->GetWidgetFromName(TEXT("Level"))))
		{
			FString JobName;
			switch (CurrentLevelIndex)
			{
			case 0:
				// Level 1
				JobName = TEXT("코인빨래방 알바");
				break;
			case 1:
				// Level 2
				JobName = TEXT("PC방 알바");
				break;
			case 2:
				// Level 3
				JobName = TEXT("물류창고 알바");
				break;
			default:
				JobName = TEXT("알바");
				break;
			}
			LevelText->SetText(FText::FromString(JobName));
		}
		// Wave - 현재 근무 시간 표시
		if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
		{
			// 전체 게임 기준 Wave 순서 계산
			const int32 GlobalWaveIndex = (CurrentLevelIndex * MaxWaves) + (CurrentWave - 1);
			// 15시부터 1시간씩 증가
			const int32 WorkHour = 15 + GlobalWaveIndex;
			// 최대 23시
			const int32 DisplayHour = FMath::Clamp(WorkHour, 15, 23);
			// 시간 표시
			WaveText->SetText(FText::FromString(FString::Printf(TEXT("%02d:00"),DisplayHour)));
		}
        // Collect - 청소할 물건 / 청소한 물건
		if (UTextBlock* CollectText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CollectText"))))
		{
			// 현재 Level에서 청소해야 하는 점수 아이템 이름
			FString CleaningItems;
			switch (CurrentLevelIndex)
			{
			case 0:
				// Level 1 - 코인빨래방
				CleaningItems = TEXT("양말 / 수건");
				break;
			case 1:
				// Level 2 - PC방
				CleaningItems = TEXT("마우스 / 키보드");
				break;
			case 2:
				// Level 3 - 물류창고
				CleaningItems = TEXT("작은 택배상자 / 큰 택배상자");
				break;
			default:
				CleaningItems = TEXT("청소할 물건");
				break;
			}
			// 청소할 물건과 현재 수집 개수 표시
			CollectText->SetText(FText::FromString(FString::Printf(TEXT("청소할 것 : %s\n" "청소한 물건 : %d / %d"), *CleaningItems, CollectedCoinCount,SpawnedCoinCount)));
		}
		// 남은 시간 ProgressBar 갱신
		if (UProgressBar* TimeProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("TimeProgressBar"))))
		{
			float RemainingTime = GetWorldTimerManager()
				.GetTimerRemaining(LevelTimerHandle);
			RemainingTime = FMath::Max(0.0f, RemainingTime);
			float TimePercent = 0.0f;
			if (LevelDuration > 0.0f)
			{
				TimePercent = RemainingTime / LevelDuration;
			}
			// 0~1 범위로 제한
			TimePercent = FMath::Clamp(TimePercent, 0.0f, 1.0f);
			TimeProgressBar->SetPercent(TimePercent);
		}
		// Player Character 가져오기
		AVODCharacter* VODCharacter = Cast<AVODCharacter>(PlayerController->GetPawn());
		if (!VODCharacter)
		{
			return;
		}
		// 현재 체력
		const float CurrentHealth = static_cast<float>(VODCharacter->GetHealth());
		// 최대 체력
		const float MaximumHealth = VODCharacter->GetMaxHealth();
		// HP 숫자 표시
		if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HPText"))))
		{
			HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.0f / %.0f"), CurrentHealth, MaximumHealth)));
		}
		// HP ProgressBar 갱신
		if (UProgressBar* HPProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HPProgressBar"))))
		{
			float HealthPercent = 0.0f;
			if (MaximumHealth > 0.0f)
			{
				HealthPercent = CurrentHealth / MaximumHealth;
			}
			// 0~1 범위로 제한
			HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f);
			HPProgressBar->SetPercent(HealthPercent);
		}
	}
}
