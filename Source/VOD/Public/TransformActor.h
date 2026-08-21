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
	ATransformActor();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "TransformActor|Actions")
	void ResetPosition();

	UFUNCTION(BlueprintPure, Category = "TransformActor|Properties")
	float GetRotationSpeed() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "TransformActor|Events")
	void OnTransformActorStarted();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TransformActor|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Movement")
	float SpeedZ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TransformActor|Rotation")
	float RotationSpeed;

	UPROPERTY()
	float ElapsedTime;
	UPROPERTY()
	float MoveDirection;
	UPROPERTY()
	float StartZ;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "TransformActor|Movement")
	float MoveRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformActor|Scale")
	float ScalePeriod;
};
