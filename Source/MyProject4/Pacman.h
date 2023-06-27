// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Custom_MapBlock.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Util.h"
#include "Pacman.generated.h"

UCLASS()
class MYPROJECT4_API APacman : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacman();

	// movement handler
	void MoveLR(float movementDelta);
	void MoveUD(float movementDelta);

	// Velocita pacman standard: 5.0 velocita al 100%
	float speed=10.0f;	

	// Variabile che si attiva quando viene effettuato il primo movimento (Serve per far attivare i fantasmi)
	bool FirstMoveFlag = false;
	// Variabile che si attiva alla morte di pacman
	bool isDead = false;
	// Vite di pacman (default a 2)
	int lives = 2;

	//TODO: Spostare in globale [a cuasa del nuovo livello]
	int point = 0;

	//Punti normali mangiati
	int rawPoints = 0;

	// Default spawn block
	ACustom_MapBlock* defaultSpawnBlock = nullptr;



	//Component di collisisone
	UCapsuleComponent* CollisionComponent;

	//Funzione per controllare se un blcco e walkable
	bool isBlockWalkable(ACustom_MapBlock& block);

	//Blocco da dove sis ta muovendo pacman
	ACustom_MapBlock* currentBlock = nullptr;
	//Blocco dove deve arrivare pacman
	ACustom_MapBlock* nextBlock = nullptr;
	
	//Se ha mangiato un cibo super si attiva
	bool isFrightnedModeOn = false;

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

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	



	Util::MovementState permanentOldState = Util::IDLE;
	Util::MovementState oldState = Util::IDLE;
	Util::MovementState movState = Util::IDLE;
	Util::MovementState bufferedMove = Util::IDLE;

	ACustom_MapBlock* getNextBlock();
	AActor* getClosestActor(const FVector& TargetLocation);

	//util
	bool isTurnPossible(Util::MovementState movement);
	bool IsBetween(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB);


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void teleportPacman(ACustom_MapBlock* teleportBlock);

	int* PointsForCompletition;
	bool* isMapCompleted;

	int mapLevel;
	bool skipMovment = false;
	double skipMovmentTimer = 0;
	float levelPacmanSpeed[21] = { 0.80,0.90,0.90,0.90,1,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0.90 };
	float levelPacmanFrightnedSpeed[21] = { 0.90,0.95,0.95,0.95,1,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};


