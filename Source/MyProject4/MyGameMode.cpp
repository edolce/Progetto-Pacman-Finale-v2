// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

// Sets default values
AMyGameMode::AMyGameMode()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameMode::BeginPlay() {


	FVector Translation = FVector::ZeroVector;
	FQuat Rotation = FQuat::Identity;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);

	FTransform MyTransform(Rotation, Translation, Scale);

	currentMap = GetWorld()->SpawnActorDeferred<ACustomMap>(customMap, MyTransform);


	currentMap->mapLevel = 1;
	currentMap->InitialMapLives = 3;
	currentMap->points = 0;
	currentMap->isMapCompleted = &isLevlFinished;

	UGameplayStatics::FinishSpawningActor(currentMap, MyTransform);
}


// Called every frame
void AMyGameMode::Tick(float DeltaTime)
{

	if (isLevlFinished) {

		int pastLevel = currentMap->mapLevel;
		int pastLives = currentMap->pacman->lives + 1;
		int pastPoint = currentMap->points;
		
		currentMap->Destroy();

		FVector Translation = FVector::ZeroVector;
		FQuat Rotation = FQuat::Identity;
		FVector Scale = FVector(1.0f, 1.0f, 1.0f);

		FTransform MyTransform(Rotation, Translation, Scale);

		APawn* PacMan = GetWorld()->GetFirstPlayerController()->GetPawn();
		PacMan->SetActorLocation(MyTransform.GetLocation());

		currentMap = GetWorld()->SpawnActorDeferred<ACustomMap>(customMap, MyTransform);

		currentMap->mapLevel = pastLevel+1;
		currentMap->InitialMapLives = pastLives;
		currentMap->points = pastPoint;
		currentMap->isMapCompleted = &isLevlFinished;

		UGameplayStatics::FinishSpawningActor(currentMap, MyTransform);
		isLevlFinished = false;


	}

}


