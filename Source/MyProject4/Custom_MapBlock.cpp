// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom_MapBlock.h"

// Sets default values
ACustom_MapBlock::ACustom_MapBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustom_MapBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustom_MapBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACustom_MapBlock::getCenterPoint() {
	if (RootComponent == nullptr) // Check if RootComponent is null
	{
		UE_LOG(LogTemp, Error, TEXT("RootComponent is null in getCenterPoint()"));
		return FVector::ZeroVector;
	}
	// Get the actor's bounding box
	FBox ActorBounds = GetComponentsBoundingBox();

	// Calculate the center of the bounding box
	return ActorBounds.GetCenter();
}

void ACustom_MapBlock::NotifyActorBeginOverlap(AActor* OtherActor) {
	//UE_LOG(LogTemp, Warning, TEXT("La pallina e stata toccata"));
}