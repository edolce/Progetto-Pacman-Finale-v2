// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFood.h"

// Sets default values
ACustomFood::ACustomFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomFood::NotifyActorBeginOverlap(AActor* OtherActor) {
}

