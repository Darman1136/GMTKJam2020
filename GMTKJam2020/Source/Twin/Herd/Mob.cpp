#include "Mob.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AMob::AMob() {
	PrimaryActorTick.bCanEverTick = true;

}

void AMob::BeginPlay() {
	Super::BeginPlay();

	AActor* FoundCenter = UGameplayStatics::GetActorOfClass(GetWorld(), AHerdCenter::StaticClass());
	if (FoundCenter) {
		HerdCenter = Cast<AHerdCenter>(FoundCenter);
	}
}

void AMob::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!HerdCenter || IsDeathTriggered()) {
		return;
	}

	FVector Target = FVector::ZeroVector;
	FVector HerdCenterLocation = HerdCenter->GetActorLocation();


	//if (FVector::Dist(HerdCenterLocation, GetActorLocation()) < 50.f) {
	//	return;
	//}

	FVector SeparationVector = FVector::ZeroVector;
	TArray<AActor*> FoundMobs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMob::StaticClass(), FoundMobs);
	for (int index = 0; index < FoundMobs.Num(); index++) {
		AMob* Mob = Cast<AMob>(FoundMobs[index]);
		if (Mob == this) {
			continue;
		}
		float Dist = FVector::Dist(GetActorLocation(), Mob->GetActorLocation());
		if (Dist > MaxDistance) {
			continue;
		}
		SeparationVector += (GetActorLocation() - Mob->GetActorLocation()) * (1. - UKismetMathLibrary::NormalizeToRange(Dist, 0, MaxDistance));
		//FVector Sub = GetActorLocation() - Mob->GetActorLocation();
		//SeparationVector += Sub * Sub.GetSafeNormal().Size();
	}
	//SeparationVector /= FoundMobs.Num() - 1;
	SeparationVector.Normalize();

	FVector TowardsCenter = HerdCenterLocation - GetActorLocation();
	TowardsCenter.Normalize();

	if (Flee) {
		TowardsCenter *= -1.f;
	}

	FVector CollisionAvoidance = FVector::ZeroVector;

	AvoidCollision(CollisionAvoidance, FVector(1, 0, 0));
	AvoidCollision(CollisionAvoidance, FVector(-1, 0, 0));
	AvoidCollision(CollisionAvoidance, FVector(0, 1, 0));
	AvoidCollision(CollisionAvoidance, FVector(0, -1, 0));

	AvoidCollision(CollisionAvoidance, FVector(1, 1, 0));
	AvoidCollision(CollisionAvoidance, FVector(-1, 1, 0));
	AvoidCollision(CollisionAvoidance, FVector(1, -1, 0));
	AvoidCollision(CollisionAvoidance, FVector(-1, -1, 0));

	FVector NewDirection = TowardsCenter + SeparationVector + CollisionAvoidance;
	UE_LOG(LogTemp, Log, TEXT("%s"), *NewDirection.ToString());
	//FVector NewDirection = SeparationVector;
	//NewDirection /= 2;
	NewDirection.Normalize();
	NewDirection *= FVector(1, 1, 0);

	LastUpdateDirection = NewDirection;

	float SpeedMultiplier = 1.f;
	if (!Flee) {
		SpeedMultiplier = UKismetMathLibrary::NormalizeToRange(FVector::Dist(HerdCenterLocation, GetActorLocation()), 0., 300.);
	}
	NewDirection *= MoveSpeed * SpeedMultiplier * DeltaTime;
	SetActorLocation(GetActorLocation() + NewDirection, true);
}


void AMob::AvoidCollision(FVector& CollisionAvoidanceAccumulator, FVector TraceDirection) {
	FVector Start = GetActorLocation() + FVector(0, 0, 50.f);
	FVector End = Start + CollisionTraceLength * TraceDirection;

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility)) {
		CollisionAvoidanceAccumulator += TraceDirection * -1.;
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.Actor->GetName())
	}
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, .1);
}

void AMob::Death() {
	if (Alive) {
		Alive = false;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.f));
		UGameplayStatics::SpawnSound2D(GetWorld(), DeathSound, .7f);
		Destroy();
	}
}
