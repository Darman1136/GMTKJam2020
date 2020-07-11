#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HerdCenter.generated.h"

UCLASS()
class TWIN_API AHerdCenter : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHerdCenter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
