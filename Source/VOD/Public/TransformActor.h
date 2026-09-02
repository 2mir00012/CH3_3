#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TransformActor.generated.h"

UCLASS()
class VOD_API ATransformActor : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	ATransformActor();
	// 매 프레임 Transform 변경
	virtual void Tick(float DeltaTime) override;
	// 위치 초기화
	UFUNCTION(BlueprintCallable, Category = "TransformActor|Actions")
	void ResetPosition();
	// 회전 속도 반환
	UFUNCTION(BlueprintPure, Category = "TransformActor|Properties")
	float GetRotationSpeed() const;
	// Blueprint에서 구현할 시작 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "TransformActor|Events")
	void OnTransformActorStarted();

protected:
	// 게임 시작
	virtual void BeginPlay() override;
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TransformActor|Components")
	USceneComponent* SceneRoot;
	// Actor Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Components")
	UStaticMeshComponent* StaticMeshComp;
	// Z축 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Movement")
	float SpeedZ;
	// 회전 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TransformActor|Rotation")
	float RotationSpeed;
	// 경과 시간
	UPROPERTY()
	float ElapsedTime;
	// 이동 방향
	UPROPERTY()
	float MoveDirection;
	// 시작 Z 위치
	UPROPERTY()
	float StartZ;
	// 이동 범위
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "TransformActor|Movement")
	float MoveRange;
	// 스케일 변화 주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Scale")
	float ScalePeriod;
};
