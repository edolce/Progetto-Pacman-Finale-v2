// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util.h"
#include "CustomFood.generated.h"

UCLASS()
class MYPROJECT4_API ACustomFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomFood();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	Util::FoodType type=Util::NORMAL;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
