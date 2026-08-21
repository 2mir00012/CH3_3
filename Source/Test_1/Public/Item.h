#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTEST_1, Warning, All);

UCLASS()
class TEST_1_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;
	UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	void ResetActorPosition();
	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;
	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Events")
	void OnItemPickedUp();

	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
};
