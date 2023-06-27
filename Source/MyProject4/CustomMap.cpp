// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMap.h"
#include "Camera/CameraComponent.h"
#include "CustomFood.h"
#include "Camera/CameraActor.h"
#include "Custom_MapBlock.h"
#include "Pacman.h"
#include "Util.h"
#include "UObject/Object.h"
#include "UObject/ConstructorHelpers.h"
#include "String.h"
#include "Blinky_Ghost.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ACustomMap::ACustomMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set the root component of this actor.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ACustomMap::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AActor> levelFruitsTEMPLATE[21] = { Cherry,StrawBerry,Peach,Peach,Apple,Apple,Grapes,Grapes,Galaxian,Galaxian,Bell,Bell,Key,Key ,Key ,Key ,Key ,Key ,Key ,Key ,Key };

	for (int i = 0; i < 21; i++) {
		levelFruits[i] = levelFruitsTEMPLATE[i];
	}

	// Fix Camera
	setFixedCamera();

	// Default Spawn Parameters

	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Create Teleport blocks
	teleportCreation();

	// Creation of game strings ("Level"/"Points"/"HighScore")
	stringCreation();

	// Lives visuals
	lifePointsVisual();

	// Creation of map (basic blocks: "path"/"walls"/ecc..)
	mapCreation();

	// Spawn ghosts
	spawnGhosts();

	//Spawn super food
	superFoodCreation();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Your Message"));
}


void ACustomMap::spawnPacman(FTransform transform, AActor* block) {
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector TranslationOffset(0, 0, 50.0f);
	transform.AddToTranslation(TranslationOffset);

	APawn* PacMan = GetWorld()->GetFirstPlayerController()->GetPawn();
	PacMan->SetActorLocation(transform.GetLocation());
	pacman = static_cast<APacman*>(PacMan);

	pacman->lives = InitialMapLives-1;
	pacman->PointsForCompletition = &(this->PointsForCompletition);
	pacman->isMapCompleted = isMapCompleted;

	pacman->currentBlock = dynamic_cast<ACustom_MapBlock*>(block);
	pacman->defaultSpawnBlock = dynamic_cast<ACustom_MapBlock*>(block);

	ResetPacman();

	UE_LOG(LogTemp, Warning, TEXT("ERRORE: PACMAN E NULL"));
}

// Called every frame
void ACustomMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	if ((isGameOver || pacman->isDead) && blinkySpawned->isActive) {
		blinkySpawned->Deactivate();
		pinkySpawned->Deactivate();
		inkySpawned->Deactivate();
		clydeSpawned->Deactivate();
	}

	if (isGameOver == true) {return;
	}

	if (GlobalCounterActivator && pacman->FirstMoveFlag) {
		timerPacmanPointEaten += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("AUMENTO TIMER [%s]"), *FString::SanitizeFloat(timerPacmanPointEaten));

		if (mapLevel > 5) {
			if (timerPacmanPointEaten > 3) {
				AGhost* prioritizedGhost = getPrioritizedGhost();
				if (prioritizedGhost != nullptr) {
					prioritizedGhost->isInCage = false;
				}
				timerPacmanPointEaten = 0;
			}
		}
		else {
			if (timerPacmanPointEaten > 4) {
				AGhost* prioritizedGhost = getPrioritizedGhost();
				if (prioritizedGhost != nullptr) {
					prioritizedGhost->isInCage = false;
				}
				timerPacmanPointEaten = 0;
			}
		}
	}

	

	//UE_LOG(LogTemp, Warning, TEXT("Current block X coordinate: %f"), pacman->currentBlock->GetActorLocation().X);

	if (pacman != nullptr) {
		if (pacman->isDead) {

			pacman->lives--;

			if (pacman->lives == -1) {

				//GAME OVER
				isGameOver = true;

				FTransform finalTransform = GetTransform();
				AActor* gameOverSpawned = GetWorld()->SpawnActor<AActor>(gameOverObject, finalTransform, spawnParams);

				return;
			}
			else {

				//reset ghost
				ResetGhosts();

				ResetPacman();
				pacman->isDead = false;
				GlobalCounterActivator = true;
			}
		}
		

		//check if pacman perform first movement
		if (pacman->FirstMoveFlag && activateFlagGhost) {
			lives[pacman->lives]->Destroy();
			lives[pacman->lives] = nullptr;
			blinkySpawned->Activate();
			pinkySpawned->Activate();
			inkySpawned->Activate();
			clydeSpawned->Activate();

			activateFlagGhost = false;
		}


		if (points != pacman->point) {
			timerPacmanPointEaten = 0;
			
			

			if (pacman->point >= points) {
				points = pacman->point;
			}
			else {
				pacman->point = points;
			}

			PointsText->SetText(FText::FromString(FString::FromInt(points)));
			if (pacman->rawPoints == 70 || pacman->rawPoints == 161) {

				if (levelFruits[mapLevel - 1] == nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("ERRORE FRUTTO NON PUNTATO"));
				}

				pacman->rawPoints++;
				//spawn fruit start dountdown
				FTransform Tranform = GetTransform();
				Tranform.AddToTranslation(FrutiSpawnLocation);
				AActor* RawFruit = GetWorld()->SpawnActor<AActor>(levelFruits[mapLevel-1], Tranform, spawnParams);
				CurrentFruit = static_cast<ACustomFood*>(RawFruit);



				CurrentFruit->type = levelFruitsType[mapLevel - 1];

				if (levelFruitsType[mapLevel - 1] == Util::CHERRY) {
					UE_LOG(LogTemp, Warning, TEXT("FRUTTO: CHERRY"));
				}
				if (levelFruitsType[mapLevel - 1] == Util::STRAWBERRY) {
					UE_LOG(LogTemp, Warning, TEXT("FRUTTO: STRAWBERRY"));

				}

				if (CurrentFruit != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("FRUIT SPAWNATO"));
				}
				FruitTimer = 10;
			}

			if (GlobalCounterActivator) {
				GlobalGhostPoints++;

				if (GlobalGhostPoints == 7) {
					AGhost* prioritizedGhost = getPrioritizedGhost();
					if (prioritizedGhost && prioritizedGhost->IsA(APinky_Ghost::StaticClass())) {
						prioritizedGhost->isInCage = false;
					}
				}

				if (GlobalGhostPoints == 17) {
					AGhost* prioritizedGhost = getPrioritizedGhost();
					if (prioritizedGhost && prioritizedGhost->IsA(AInky_Ghost::StaticClass())) {
						prioritizedGhost->isInCage = false;
					}
				}

				if (GlobalGhostPoints == 32) {
					GlobalGhostPoints = 0;
					GlobalCounterActivator = false;
				}


			}
			else {
				AGhost* prioritizedGhost = getPrioritizedGhost();
				if (prioritizedGhost != nullptr) {
					if (prioritizedGhost->pointCounter != prioritizedGhost->firstExitLimitPoint) {
						UE_LOG(LogTemp, Warning, TEXT("AUMENTO COUNTER FANTASMA"));
						prioritizedGhost->pointCounter++;
					}
				}
			}
		}	
	}

	

	if (CurrentFruit != nullptr) {
		if (FruitTimer < 0) {

			UE_LOG(LogTemp, Warning, TEXT("FRUIT DISTRUTTO"));
			CurrentFruit->Destroy();
			CurrentFruit = nullptr;
		}
		else {
			FruitTimer -= DeltaTime;
		}
	}
	if (!GlobalCounterActivator) {
		AGhost* prioritizedGhost = getPrioritizedGhost();
		if (prioritizedGhost != nullptr) {
			if (prioritizedGhost->pointCounter == prioritizedGhost->firstExitLimitPoint) {
				prioritizedGhost->isInCage = false;
				UE_LOG(LogTemp, Warning, TEXT("Il fantasma %s e uscito dalla cage"), *prioritizedGhost->GetName());
			}

		}
	}

}

AGhost* ACustomMap::getPrioritizedGhost() {
	for (int i = 0; i < 4; i++) {
		if (ghosts[i] == nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("IL FANTASMA NON E ANCORA STATO CREATO"));
			continue;
		}
		if (!ghosts[i]->isActive) continue;

		if (!ghosts[i]->isInCage) continue;

		return ghosts[i];
	}
	//UE_LOG(LogTemp, Warning, TEXT("NESSUN FANTASMA PRIORIZZATO PER L'USCITA"));

	return nullptr;
}


//Setting the fixed camera
void ACustomMap::setFixedCamera() {
	// Camera creation
	ACameraActor* MyFixedCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
	
	MyFixedCamera->SetActorLocation(FVector(-50.f, 0.f, 3500.f)); // Impostazione della posizione della telecamera 
	MyFixedCamera->SetActorRotation(FRotator(-90.f, -90.f, 0.f)); // Impostazione della rotazione della telecamera 
	// Impostazione della telecamera fissa come telecamera di default 
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(MyFixedCamera, 0.f);
}


// Create The teleport blocks
void ACustomMap::teleportCreation() {
	AActor* teleports[2] = { nullptr };

	//1 teleport
	FVector TeleportOffSetLocation((0 - 14) * 100, (17 - 18) * 100, 00.0f);
	FTransform TelportTransform = GetTransform();
	TelportTransform.AddToTranslation(TeleportOffSetLocation);
	teleports[0] = GetWorld()->SpawnActor<AActor>(teleport, TelportTransform, spawnParams);

	//2 teleport
	FVector TeleportOffSetLocation2((27 - 14) * 100, (17 - 18) * 100, 00.0f);
	FTransform TelportTransform2 = GetTransform();
	TelportTransform2.AddToTranslation(TeleportOffSetLocation2);
	teleports[1] = GetWorld()->SpawnActor<AActor>(teleport, TelportTransform2, spawnParams);

	ACustom_MapBlock* castedTeleport1 = dynamic_cast<ACustom_MapBlock*>(teleports[0]);
	ACustom_MapBlock* castedTeleport2 = dynamic_cast<ACustom_MapBlock*>(teleports[1]);

	castedTeleport1->x = 0;
	castedTeleport1->y = 17;

	castedTeleport2->x = 27;
	castedTeleport2->y = 17;

		castedTeleport1->teleportDestination = castedTeleport2;
		castedTeleport1->direction = Util::LEFT;
		castedTeleport2->teleportDestination = castedTeleport1;
		castedTeleport2->direction = Util::RIGHT;
	
		blocksMap[17][0] = castedTeleport1;
		blocksMap[17][27] = castedTeleport2;
}

// Creation of game strings ("Level"/"Points"/"HighScore"/"Lives")
void ACustomMap::stringCreation() {
	FVector LevelStringLocation((3 - 14) * 100, (0 - 18) * 100, 00.0f);
	FTransform StringTransform = GetTransform();
	StringTransform.AddToTranslation(LevelStringLocation);

	//LEVEL STRING
	LvlComponentActor = NewObject<UTextRenderComponent>(this, TEXT("LevelTEXT"));
	modifyText(LvlComponentActor, "LVL " + FString::FromInt(mapLevel), FVector(-742, -1473, 691));

	//HIGH SCORE TITLE STRING
	HighScoreTilteText = NewObject<UTextRenderComponent>(this, TEXT("HighScoreTitleTEXT"));
	modifyText(HighScoreTilteText, "HIGH SCORE", FVector(221, -1473, 691));

	//PUNTEGGIO
	PointsText = NewObject<UTextRenderComponent>(this, TEXT("PointTEXT"));
	modifyText(PointsText, "0", FVector(-796, -1391,  691));

	//HIGH SCORE
	HighScoreText = NewObject<UTextRenderComponent>(this, TEXT("HighScoreTEXT"));
	modifyText(HighScoreText, "0", FVector(179, -1391, 691));
}


void ACustomMap::modifyText(UTextRenderComponent* component,FString displayString,FVector location) {
	component->SetText(FText::FromString(displayString));
	component->SetHorizontalAlignment(EHorizTextAligment::EHTA_Right);
	component->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	component->SetWorldSize(125.0f);
	component->SetWorldLocation(location);
	component->SetWorldRotation(FRotator(90, 90, 0));
	component->SetHorizSpacingAdjust(5);

	// Add the component to the actor and register it with the scene
	component->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	component->RegisterComponent();
}


// Creation of map (basic blocks: "path"/"walls"/ecc..)
void ACustomMap::mapCreation() {

	for (int i = -18; i < 18; i++) {
		for (int k = -14; k < 14; k++) {

			TSubclassOf<AActor> mapPieceToSpawn;



			int x = k;
			int y = i;

			FVector TranslationOffset(x * 100, y * 100, 0.0f); // specifica la traslazione di 10 unità sull'asse X e 10 unità sull'asse Y

			FTransform finalTransform = GetTransform();



			finalTransform.AddToTranslation(TranslationOffset); // trasla ActorTransform di TranslationOffset

			switch (arrayMap[i + 18][k + 14])
			{
			case 'w':
				mapPieceToSpawn = wall;
				break;
			case 'p':
				mapPieceToSpawn = path;
				break;
			case 'f':
				mapPieceToSpawn = path;
				break;
			case 'v':
				mapPieceToSpawn = voidPiece;
				break;
			case 'x':
				mapPieceToSpawn = path;
				break;
			case 't':
				continue;
			case 'r':
				mapPieceToSpawn = doorGate;
				break;
			default:
				mapPieceToSpawn = path;
				break;
			}


			AActor* mapPieceSpawned = GetWorld()->SpawnActor<AActor>(mapPieceToSpawn, finalTransform, spawnParams);

			//aggiunta blocchi confinanti
			ACustom_MapBlock*  parsedMapPiece = dynamic_cast<ACustom_MapBlock*>(mapPieceSpawned);

			parsedMapPiece->x = k + 14;
			parsedMapPiece->y = i + 18;
			

			if (parsedMapPiece == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("ERRORE MAIN: SELECTED BLOCK E NULL,%d,%d"), i + 18, k + 14);
			}



			blocksMap[i + 18][k + 14] = parsedMapPiece;


			if (arrayMap[i + 18][k + 14] == 'x') {
				spawnPacman(finalTransform, mapPieceSpawned);
			}
			if (arrayMap[i + 18][k + 14] == 'R') {
				ghostsCustomBlocks[0] = parsedMapPiece;
			}
			if (arrayMap[i + 18][k + 14] == 'L') {
				ghostsCustomBlocks[1] = parsedMapPiece;
				UE_LOG(LogTemp, Warning, TEXT("L TROVATA"));
			}

			if (arrayMap[i + 18][k + 14] == 'A') {
				ghostsCustomBlocks[2] = parsedMapPiece;
			}

			if (arrayMap[i + 18][k + 14] == 'G') {
				ghostsCustomBlocks[3] = parsedMapPiece;
			}


			if (arrayMap[i + 18][k + 14] == 'f') {
				FVector PointOffSett(0, 0, 30.0f); // specifica la traslazione di 10 unità sull'asse X e 10 unità sull'asse Y
				FTransform pointTransform = finalTransform;
				pointTransform.AddToTranslation(PointOffSett); // trasla ActorTransform di TranslationOffset
				GetWorld()->SpawnActor<AActor>(point, pointTransform, spawnParams);
			}



		}
	}

	int tester = 0;

	for (int i = 0; i < 36; i++) {
		for (int k = 0; k < 28; k++) {


			ACustom_MapBlock*  selectedBlock = blocksMap[i][k];

			if (selectedBlock == nullptr) {

				UE_LOG(LogTemp, Warning, TEXT("ERRORE: SELECTED BLOCK E NULL,%d,%d"), i, k);
				return;
			}

			//UP i:-1 k:0
			if (i - 1 >= 0) {
				selectedBlock->sideBlocks[0] = blocksMap[i-1][k];
			}
			else {
				selectedBlock->sideBlocks[0] = nullptr;
				tester++;
			}

			//LEFT i:0 k:-1
			if (k - 1 >= 0) {
				selectedBlock->sideBlocks[1] = blocksMap[i][k-1];
			}
			else {
				selectedBlock->sideBlocks[1] = nullptr;
				tester++;
			}

			//DOWN i:1 k:0
			if (i + 1 < 36) {
				selectedBlock->sideBlocks[2] = blocksMap[i+1][k];
			}
			else {
				selectedBlock->sideBlocks[2] = nullptr;
				tester++;
			}

			//RIGHT i:0 k:1
			if (k + 1 < 28) {
				selectedBlock->sideBlocks[3] = blocksMap[i][k+1];
			}
			else {
				selectedBlock->sideBlocks[3] = nullptr;
				tester++;
			}

		}
	}

	UE_LOG(LogTemp, Warning, TEXT("NUMERO DI NULLPTR LATI: %d"), tester);
}

void ACustomMap::superFoodCreation() {
	FVector PointOffSett(0, 0, 30.0f);


	//FOOD 1
	FVector FoodOffSetLocation((1 - 14) * 100, (6 - 18) * 100, 00.0f);
	FTransform FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food1 = GetWorld()->SpawnActor<AActor>(superFood, FoodTransform, spawnParams);
	ACustomFood* parsedFood1 = static_cast<ACustomFood*>(food1);
	parsedFood1->type = Util::SUPER;

	//FOOD 2
	FoodOffSetLocation = FVector((1 - 14) * 100, (26 - 18) * 100, 00.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food2 = GetWorld()->SpawnActor<AActor>(superFood, FoodTransform, spawnParams);
	ACustomFood* parsedFood2 = static_cast<ACustomFood*>(food2);
	parsedFood2->type = Util::SUPER;

	//FOOD 3
	FoodOffSetLocation = FVector((26 - 14) * 100, (6 - 18) * 100, 00.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food3 = GetWorld()->SpawnActor<AActor>(superFood, FoodTransform, spawnParams);
	ACustomFood* parsedFood3 = static_cast<ACustomFood*>(food3);
	parsedFood3->type = Util::SUPER;

	//FOOD 4
	FoodOffSetLocation = FVector((26 - 14) * 100, (26 - 18) * 100, 00.0f);
	FoodTransform = GetTransform();
	FoodTransform.AddToTranslation(FoodOffSetLocation);
	AActor* food4 = GetWorld()->SpawnActor<AActor>(superFood, FoodTransform, spawnParams);
	ACustomFood* parsedFood4 = static_cast<ACustomFood*>(food4);
	parsedFood4->type = Util::SUPER;

}

void ACustomMap::lifePointsVisual() {

	FTransform lifeTransform = GetTransform();

	if (InitialMapLives >= 1) {
		lifeTransform.SetLocation(FVector(-1300, 1600, 50));
		AActor* life1 = GetWorld()->SpawnActor<AActor>(pacmanLife, lifeTransform, spawnParams);
		lives[0] = life1;
	}


	if (InitialMapLives >= 2) {
		lifeTransform = GetTransform();
		lifeTransform.SetLocation(FVector(-1200, 1600, 50));
		AActor* life2 = GetWorld()->SpawnActor<AActor>(pacmanLife, lifeTransform, spawnParams);
		lives[1] = life2;
	}

	if (InitialMapLives >= 3) {
		lifeTransform = GetTransform();
		lifeTransform.SetLocation(FVector(-1100, 1600, 50));
		AActor* life3 = GetWorld()->SpawnActor<AActor>(pacmanLife, lifeTransform, spawnParams);
		lives[2] = life3;
	}
}

// Spawn ghosts
void ACustomMap::spawnGhosts() {
	//RED GHOST
	//-100,0 -400,0 0
	FTransform blinkyTransform = GetTransform();

	blinkyTransform.SetLocation(FVector(-100, - 400, 50));

	AActor* blinkySpawnedActor = GetWorld()->SpawnActor<AActor>(blinkyGhostToSpawn, blinkyTransform, spawnParams);

	blinkySpawned = dynamic_cast<ABlinky_Ghost*>(blinkySpawnedActor);
	blinkySpawned->firstExitLimitPoint = 0;
	blinkySpawned->pacman = pacman;
	blinkySpawned->isInCage = false;
	blinkySpawned->currentBlock = ghostsCustomBlocks[0];
	blinkySpawned->defaultExitCageBlock = ghostsCustomBlocks[0];
	blinkySpawned->scatterBlock = blocksMap[0][25];
	blinkySpawned->contatoreFantasmiMangiati = &contatoreFantasmiMangiati;
	blinkySpawned->points = &points;
	UE_LOG(LogTemp, Warning, TEXT("BLINKY ATTIVATO"));

	//Routes in cage variables
	blinkySpawned->ForcedRouteIndex = -1;

	ghosts[0] = blinkySpawned;


	
	//PINKY GHOST
	//-100,0 -400,0 0
	FTransform pinkyTransform = GetTransform();

	pinkyTransform.SetLocation(FVector(-50, -100, 50));

	AActor* pinkySpawnedActor = GetWorld()->SpawnActor<AActor>(pinkyGhostToSpawn, pinkyTransform, spawnParams);

	pinkySpawned = dynamic_cast<APinky_Ghost*>(pinkySpawnedActor);
	pinkySpawned->firstExitLimitPoint = 0;
	pinkySpawned->pacman = pacman;
	pinkySpawned->currentBlock = ghostsCustomBlocks[1];
	pinkySpawned->defaultExitCageBlock = ghostsCustomBlocks[0];
	pinkySpawned->scatterBlock = blocksMap[0][2];
	UE_LOG(LogTemp, Warning, TEXT("PINKY ATTIVATO"));

	//Routes in cage variables
	pinkySpawned->movState = Util::UP;
	pinkySpawned->ForcedRouteIndex = 0;
	pinkySpawned->ForcedRoute[0] = FVector(-50, -200, 50);
	pinkySpawned->ForcedRoute[1] = FVector(-50, -100, 50);
	pinkySpawned->ForcedRoute[2] = FVector(-50, 0, 50);
	pinkySpawned->ForcedRoute[3] = FVector(-50, -100, 50);
	pinkySpawned->ExitRoute[0] = FVector(-50, -100, 50);
	pinkySpawned->ExitRoute[1] = FVector(-50, -100, 50);
	pinkySpawned->ExitRoute[2] = FVector(-50, -200, 50);
	pinkySpawned->ExitRoute[3] = FVector(-50, -300, 50);
	pinkySpawned->ExitRoute[4] = FVector(-50, -400, 50);
	pinkySpawned->ExitRoute[5] = FVector(-100, -400, 50);
	pinkySpawned->ForcedExitMovement[1] = Util::IDLE;
	pinkySpawned->contatoreFantasmiMangiati = &contatoreFantasmiMangiati;
	pinkySpawned->points = &points;


	ghosts[1] = pinkySpawned;

	
	//INKY GHOST
	//-100,0 -400,0 0
	FTransform inkyTransform = GetTransform();

	inkyTransform.SetLocation(FVector(-250, -100, 50));

	AActor* inkySpawnedActor = GetWorld()->SpawnActor<AActor>(inkyGhostToSpawn, inkyTransform, spawnParams);

	inkySpawned = dynamic_cast<AInky_Ghost*>(inkySpawnedActor);
	inkySpawned->firstExitLimitPoint = 30;
	inkySpawned->pacman = pacman;
	inkySpawned->blinky = blinkySpawned;

	
	for (int x = 0; x < 36;x++) {
		for (int y = 0;y< 28; y++) {
			inkySpawned->blocksMap[x][y] = blocksMap[x][y];
		}
	}

	inkySpawned->currentBlock = ghostsCustomBlocks[2];
	inkySpawned->defaultExitCageBlock = ghostsCustomBlocks[0];
	inkySpawned->scatterBlock = blocksMap[34][27];
	UE_LOG(LogTemp, Warning, TEXT("INKY ATTIVATO"));

	//Routes in cage variables
	inkySpawned->movState = Util::UP;
	inkySpawned->ForcedRouteIndex = 0;
	inkySpawned->ForcedRoute[0] = FVector(-250, -200, 50);
	inkySpawned->ForcedRoute[1] = FVector(-250, -100, 50);
	inkySpawned->ForcedRoute[2] = FVector(-250, 0, 50);
	inkySpawned->ForcedRoute[3] = FVector(-250, -100, 50);
	inkySpawned->ExitRoute[0] = FVector(-250, -100, 50);
	inkySpawned->ExitRoute[1] = FVector(-50, -100, 50);
	inkySpawned->ExitRoute[2] = FVector(-50, -200, 50);
	inkySpawned->ExitRoute[3] = FVector(-50, -300, 50);
	inkySpawned->ExitRoute[4] = FVector(-50, -400, 50);
	inkySpawned->ExitRoute[5] = FVector(-100, -400, 50);
	inkySpawned->ForcedExitMovement[1] = Util::RIGHT;
	inkySpawned->contatoreFantasmiMangiati = &contatoreFantasmiMangiati;
	inkySpawned->points = &points;


	ghosts[2] = inkySpawned;



	
	//CLYDE GHOST
	//-100,0 -400,0 0
	FTransform clydeTransform = GetTransform();

	clydeTransform.SetLocation(FVector(150, -100, 50));

	AActor* clydeSpawnedActor = GetWorld()->SpawnActor<AActor>(clydeGhostToSpawn, clydeTransform, spawnParams);

	clydeSpawned = dynamic_cast<AClyde_Ghost*>(clydeSpawnedActor);
	clydeSpawned->firstExitLimitPoint = 60;
	clydeSpawned->pacman = pacman;
	clydeSpawned->currentBlock = ghostsCustomBlocks[3];
	clydeSpawned->defaultExitCageBlock = ghostsCustomBlocks[0];
	clydeSpawned->scatterBlock = blocksMap[34][0];
	UE_LOG(LogTemp, Warning, TEXT("ATTIVATO"));

	//Routes in cage variables
	clydeSpawned->movState = Util::UP;
	clydeSpawned->ForcedRouteIndex = 0;
	clydeSpawned->ForcedRoute[0] = FVector(150, -200, 50);
	clydeSpawned->ForcedRoute[1] = FVector(150, -100, 50);
	clydeSpawned->ForcedRoute[2] = FVector(150, 0, 50);
	clydeSpawned->ForcedRoute[3] = FVector(150, -100, 50);
	clydeSpawned->ExitRoute[0] = FVector(150, -100, 50);
	clydeSpawned->ExitRoute[1] = FVector(-50, -100, 50);
	clydeSpawned->ExitRoute[2] = FVector(-50, -200, 50);
	clydeSpawned->ExitRoute[3] = FVector(-50, -300, 50);
	clydeSpawned->ExitRoute[4] = FVector(-50, -400, 50);
	clydeSpawned->ExitRoute[5] = FVector(-100, -400, 50);
	clydeSpawned->ForcedExitMovement[1] = Util::LEFT;
	clydeSpawned->contatoreFantasmiMangiati = &contatoreFantasmiMangiati;
	clydeSpawned->points = &points;



	ghosts[3] = clydeSpawned;
	
}


void ACustomMap::ResetGhosts() {

	blinkySpawned->SetActorLocation(FVector(-100, -400, 50));
	blinkySpawned->isInCage = false;
	blinkySpawned->currentBlock = ghostsCustomBlocks[0];
	blinkySpawned->ForcedRouteIndex = -1;
	blinkySpawned->movState = Util::LEFT;
	blinkySpawned->nextMove = Util::LEFT;
	blinkySpawned->movmentState = Util::SCATTER;

	pinkySpawned->SetActorLocation(FVector(-50, -100, 50));
	pinkySpawned->isInCage = true;
	pinkySpawned->isInCageOld = true;
	pinkySpawned->currentBlock = ghostsCustomBlocks[1];
	pinkySpawned->ForcedRouteIndex = 0;
	pinkySpawned->movState = Util::UP;
	pinkySpawned->nextMove = Util::LEFT;
	pinkySpawned->movmentState = Util::SCATTER;

	inkySpawned->SetActorLocation(FVector(-250, -100, 50));
	inkySpawned->isInCage = true;
	inkySpawned->isInCageOld = true;
	inkySpawned->currentBlock = ghostsCustomBlocks[2];
	inkySpawned->ForcedRouteIndex = 0;
	inkySpawned->movState = Util::UP;
	inkySpawned->nextMove = Util::LEFT;
	inkySpawned->movmentState = Util::SCATTER;

	clydeSpawned->SetActorLocation(FVector(150, -100, 50));
	clydeSpawned->isInCage = true;
	clydeSpawned->isInCageOld = true;
	clydeSpawned->currentBlock = ghostsCustomBlocks[3];
	clydeSpawned->ForcedRouteIndex = 0;
	clydeSpawned->movState = Util::UP;
	clydeSpawned->nextMove = Util::LEFT;
	clydeSpawned->movmentState = Util::SCATTER;
}

void ACustomMap::ResetPacman() {
	pacman->SetActorLocation(FVector(-100,800,50));
	pacman->SetActorHiddenInGame(false);
	pacman->movState = Util::IDLE;
	pacman->bufferedMove = Util::IDLE;
	pacman->currentBlock = pacman->defaultSpawnBlock;
	pacman->nextBlock = nullptr;
	pacman->FirstMoveFlag = false;
	pacman->isFrightnedModeOn = false;
	pacman->rawPoints = 0;
	pacman->mapLevel = mapLevel;
	pacman->speed = pacman->levelPacmanSpeed[mapLevel-1];
	activateFlagGhost = true;
}

void ACustomMap::Destroy() {

	Super::Destroy();

	for (int x = 0; x < 36;x++) {
		for (int y = 0; y < 28; y++) {
			blocksMap[x][y]->Destroy();
		}
	}

	blinkySpawned->Destroy();
	pinkySpawned->Destroy();
	inkySpawned->Destroy();
	clydeSpawned->Destroy();

	if(lives[0]!=nullptr)
		lives[0]->Destroy();
	
	if(lives[1]!=nullptr)
		lives[1]->Destroy();

	if(lives[2]!=nullptr)
		lives[2]->Destroy();


}