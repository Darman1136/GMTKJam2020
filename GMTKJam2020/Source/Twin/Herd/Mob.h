#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HerdCenter.h"
#include "Mob.generated.h"

UCLASS()
class TWIN_API AMob : public AActor {
	GENERATED_BODY()

public:
	AMob();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void Death();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AvoidCollision(FVector& CollisionAvoidanceAccumulator, FVector TraceDirection);

	UFUNCTION(BlueprintCallable)
		bool IsAlive() const { return bAlive; };

	UFUNCTION(BlueprintCallable)
		bool IsDeathTriggered() const { return DeathTriggered; };

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 320.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxDistance = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Flee = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CollisionTraceLength = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
		UParticleSystem* DeathEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
		USoundBase* DeathSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector LastUpdateDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool DeathTriggered = false;

private:
	AHerdCenter* HerdCenter;

	bool bAlive = true;

	APawn* Player;
};
