// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pacman.h"
#include "Components/TextRenderComponent.h"
#include "Ghost.h"
#include "CustomFood.h"
#include "Blinky_Ghost.h"
#include "Pinky_Ghost.h"
#include "Inky_Ghost.h"
#include "Clyde_Ghost.h"
#include "CustomMap.generated.h"

UCLASS()
class MYPROJECT4_API ACustomMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomMap();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> wall;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> voidPiece;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> path;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> doorGate;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> point;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> teleport;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> superFood;
	UPROPERTY(EditAnywhere, Category = "Game Over")
		TSubclassOf<AActor> gameOverObject;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* LvlComponentActor;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* HighScoreTilteText;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* HighScoreText;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* PointsText;
	UPROPERTY(EditAnywhere,Category="Ghosts")
		TSubclassOf<AActor> blinkyGhostToSpawn;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> pinkyGhostToSpawn;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> inkyGhostToSpawn;
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		TSubclassOf<AActor> clydeGhostToSpawn;

	UPROPERTY(EditAnywhere, Category= "Life")
		TSubclassOf<AActor> pacmanLife;

	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Cherry;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> StrawBerry;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Peach;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Apple;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Grapes;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Galaxian;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Bell;
	UPROPERTY(EditAnywhere, Category = "Fruits")
		TSubclassOf<AActor> Key;



	ABlinky_Ghost* blinkySpawned = nullptr;
	APinky_Ghost* pinkySpawned = nullptr;
	AInky_Ghost* inkySpawned = nullptr;
	AClyde_Ghost* clydeSpawned = nullptr;

	// move into pacman or map
	bool isGameOver = false;
	bool activateFlagGhost = true;

	int InitialMapLives = 3;

	AActor* lives[3];

	// Contatore tempo per uscita ghost pacman morto
	float timerPacmanPointEaten=0;

	AGhost* ghosts[4];

	AGhost* getPrioritizedGhost();

	APacman* pacman = nullptr;

	int mapLevel = 1;

	int points=0;

	int contatoreFantasmiMangiati = 0;

	int GlobalGhostPoints = 0;
	bool GlobalCounterActivator = false;

	void spawnGhosts();

	void ResetGhosts();

	void ResetPacman();

	ACustom_MapBlock* ghostsCustomBlocks[4];
	bool* isMapCompleted;

protected:
	void spawnPacman(FTransform transform, AActor* block);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Destroy();

private:
	void setFixedCamera();

	FActorSpawnParameters spawnParams;

	/*
	//v = vuoto
	//w = muro
	//sentiero senza cibo = p
	//sentieri con cibo = f
	//super cubo = s
	//rosa gabbia = r
	//tunnel = t
	//gabbia = g 
	// STRINGA LIVELLO = L
	// STRING "HIGH SCORE" = H
	// PUNTEGGIO RECORD = R
	*/
	char arrayMap[36][28] = {
	{ 'v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v'},
	{ 'v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v' },
	{ 'v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v' },
	{ 'w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w' },
	{ 'w','f','f','f','f','f','f','f','f','f','f','f','f','w','w','f','f','f','f','f','f','f','f','f','f','f','f','w' },
	{ 'w','f','w','w','w','w','f','w','w','w','w','w','f','w','w','f','w','w','w','w','w','f','w','w','w','w','f','w' },
	{ 'w','s','w','w','w','w','f','w','w','w','w','w','f','w','w','f','w','w','w','w','w','f','w','w','w','w','s','w' },
	{ 'w','f','w','w','w','w','f','w','w','w','w','w','f','w','w','f','w','w','w','w','w','f','w','w','w','w','f','w' },
	{ 'w','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','w' },
	{ 'w','f','w','w','w','w','f','w','w','f','w','w','w','w','w','w','w','w','f','w','w','f','w','w','w','w','f','w' },
	{ 'w','f','w','w','w','w','f','w','w','f','w','w','w','w','w','w','w','w','f','w','w','f','w','w','w','w','f','w' },
	{ 'w','f','f','f','f','f','f','w','w','f','f','f','f','w','w','f','f','f','f','w','w','f','f','f','f','f','f','w' },
	{ 'w','w','w','w','w','w','f','w','w','w','w','w','p','w','w','p','w','w','w','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','w','w','w','p','w','w','p','w','w','w','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','p','p','p','R','p','p','p','p','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','w','w','r','r','w','w','w','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','g','G','g','L','g','A','w','p','w','w','f','w','w','w','w','w','w' },
	{ 't','p','p','p','p','p','f','p','p','p','w','g','g','g','g','g','g','w','p','p','p','f','p','p','p','p','p','t' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','g','g','g','g','g','g','w','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','w','w','w','w','w','w','w','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','p','p','p','p','p','p','p','p','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','w','w','w','w','w','w','w','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','w','w','w','w','w','f','w','w','p','w','w','w','w','w','w','w','w','p','w','w','f','w','w','w','w','w','w' },
	{ 'w','f','f','f','f','f','f','f','f','f','f','f','f','w','w','f','f','f','f','f','f','f','f','f','f','f','f','w' },
	{ 'w','f','w','w','w','w','f','w','w','w','w','w','f','w','w','f','w','w','w','w','w','f','w','w','w','w','f','w' },
	{ 'w','f','w','w','w','w','f','w','w','w','w','w','f','w','w','f','w','w','w','w','w','f','w','w','w','w','f','w' },
	{ 'w','s','f','f','w','w','f','f','f','f','f','f','f','x','p','f','f','f','f','f','f','f','w','w','f','f','s','w' },
	{ 'w','w','w','f','w','w','f','w','w','f','w','w','w','w','w','w','w','w','f','w','w','f','w','w','f','w','w','w' },
	{ 'w','w','w','f','w','w','f','w','w','f','w','w','w','w','w','w','w','w','f','w','w','f','w','w','f','w','w','w' },
	{ 'w','f','f','f','f','f','f','w','w','f','f','f','f','w','w','f','f','f','f','w','w','f','f','f','f','f','f','w' },
	{ 'w','f','w','w','w','w','w','w','w','w','w','w','f','w','w','f','w','w','w','w','w','w','w','w','w','w','f','w' },
	{ 'w','f','w','w','w','w','w','w','w','w','w','w','f','w','w','f','w','w','w','w','w','w','w','w','w','w','f','w' },
	{ 'w','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','f','w' },
	{ 'w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w','w' },
	{ 'v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v' },
	{ 'v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v','v' },
	};

	ACustom_MapBlock* blocksMap[36][28];
	void teleportCreation();

	// Creation of game strings ("Level"/"Points"/"HighScore")
	void stringCreation();

	// Creation of map (basic blocks: "path"/"walls"/ecc..)
	void mapCreation();

	// Creation of super food
	void superFoodCreation();

	//UTILS
	void modifyText(UTextRenderComponent* component, FString displayString, FVector location);

	void lifePointsVisual();
	

	//Fruits
	TSubclassOf<AActor> levelFruits[21];
	Util::FoodType levelFruitsType[21] = {Util::CHERRY,Util::STRAWBERRY,Util::PEACH,Util::PEACH ,Util::APPLE,Util::APPLE,Util::GRAPES,Util::GRAPES,Util::GALAXIAN,Util::GALAXIAN,Util::BELL,Util::BELL,Util::KEY,Util::KEY,Util::KEY ,Util::KEY ,Util::KEY ,Util::KEY ,Util::KEY ,Util::KEY ,Util::KEY };

	ACustomFood* CurrentFruit = nullptr;;
	float FruitTimer=0;

	FVector FrutiSpawnLocation = FVector(-50, 800,100);

	int PointsForCompletition = 244;
	UTextRenderComponent* TextRenderComponent;

};
