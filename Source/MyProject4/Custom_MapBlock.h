// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util.h"
#include "Custom_MapBlock.generated.h"

UCLASS()
class MYPROJECT4_API ACustom_MapBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustom_MapBlock();

	UPROPERTY(EditAnywhere, Category = "Walkable")
		bool isWalkable = false;
	UPROPERTY(EditAnywhere, Category = "Telport")
		bool isTeleport = false;
	UPROPERTY(EditAnywhere, Category = "Telport")
		ACustom_MapBlock* teleportDestination;
	UPROPERTY(EditAnywhere, Category = "Telport")
		bool isSlowToApply = false;

	Util::MovementState direction = Util::IDLE;


	// UP LEFT DOWN RIGHT
	ACustom_MapBlock* sideBlocks[4];

	//Get global coordinates of the center point of the block
	FVector getCenterPoint();

	int x=-1;
	int y=-1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
