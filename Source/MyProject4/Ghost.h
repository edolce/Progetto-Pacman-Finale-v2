// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom_MapBlock.h"
#include "Util.h"
#include "Components/CapsuleComponent.h"
#include "Pacman.h"
#include "Ghost.generated.h"

UCLASS()
class MYPROJECT4_API AGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	AGhost();
	// Sets default values for this actor's properties
	AGhost(int mapLevel,APacman* pacman);
	APacman* pacman;
	//DA CAMBIARE QUANDO IMPLEMENTIAMO I LIVELLI
	int mapLevel=1;
	void Activate();
	void Deactivate();
	void ForcedMovement(FVector movementVelocityVector);

	bool isActive = false;

	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* frightnedMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* flashFrightnedMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* defaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* deadMaterial;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UStaticMeshComponent* MeshComp;
	UStaticMeshComponent* MeshComp2;

	// Cage route variables
	FVector ForcedRoute[4];
	Util::MovementState ForcedRouteMovement[4] = { Util::UP, Util::DOWN,Util::DOWN,Util::UP };
	FVector ExitRoute[6];
	Util::MovementState ForcedExitMovement[6] = {Util::UP,Util::RIGHT, Util::UP,Util::UP, Util::UP,Util::LEFT };
	int ForcedRouteIndex = 0;
	bool isInCage = true;
	bool isInCageOld = true;
	bool isDead = false;

	bool IsBetween(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB);

	float speed = 5.0f;

	virtual void check_for_target_change();
	Util::MovementState next_move();
	Util::MovementState next_move_frightned();
	ACustom_MapBlock* getNextBlock(ACustom_MapBlock* block, Util::MovementState movState);

	ACustom_MapBlock* currentBlock = nullptr;
	ACustom_MapBlock* defaultExitCageBlock = nullptr;
	ACustom_MapBlock* scatterBlock = nullptr;
	ACustom_MapBlock* target = nullptr;


	//movmente variables
	Util::GhostState movmentState = Util::SCATTER;
	double frightnedTimer = 0;
	int movmentCycleIndicator = 0;
	double timeIndicator = 0;
	double secondsPassed = 0;
	bool invertMovementFlag = false;

	float flashIndicatorTime = 0;


	int levelFrightnedTime[21] = {
		6,5,4,3,2,5,2,2,1,5,2,1,1,3,1,1,0,1,0,0,0
	};

	int  *contatoreFantasmiMangiati;
	int *points;

	//levelMovmentModeTABLE[0][] - Level 1
	//levelMovmentModeTABLE[1][] - Level 2-4
	//levelMovmentModeTABLE[2][] - Level 5+

	float levelMovmentModeTABLE[3][8] = {
		{7,20,7,20,5,20,5,-1},
		{7,20,7,20,5,1033,1/60,-1},
		{7,20,7,20,5,1037,1/60,-1}
	};

	Util::GhostState oldMovementState = Util::SCATTER;

	FString enumToString(Util::MovementState value)
	{
		switch (value)
		{
		case Util::MovementState::IDLE: return "IDLE";
		case Util::MovementState::LEFT: return "LEFT";
		case Util::MovementState::RIGHT: return "RIGHT";
		case Util::MovementState::UP: return "UP";
		case Util::MovementState::DOWN: return "DOWN";
		default: return "Unknown";
		}
	}

	int HELP = 0;

	//Util::MovementState oldState = Util::IDLE;
	Util::MovementState movState = Util::LEFT;
	Util::MovementState nextMove = Util::LEFT;

	//bool isTurnPossible(Util::MovementState movement);


	FString color;

	// ALL SPAWN VARIABLES
	// spawn position of the ghost
	ACustom_MapBlock* spawnPosition;
	// exit priority from the cage
	int priority;
	//
	int firstExitLimitPoint;
	int globalLimitPoint;

	int pointCounter=0;
	void teleportGhost(ACustom_MapBlock* teleportBlock);

	//ALL LEVEL DEPENDENT VARIABLE
	float levelGhostSpeed[21] = { 0.75,0.85,0.85,0.85,0.95,0.95,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 ,0.95 };
	float levelGhostFrightnedSpeed[21] = { 0.50,0.55,0.55,0.55,0.60,0.60,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,0.60 ,1 ,0.60 ,1 ,1 ,1 };



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
