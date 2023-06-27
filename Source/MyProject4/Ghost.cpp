// Fill out your copyright notice in the Description page of Project Settings.

/*
FUNZIONI CONDIVISE:
eventoCollisionePacman(); [FEAR->morte();/NON FEAR->killPacman();]
virtual movimento(); [differente per ognuno] [FRIGHTENED(uguale)/CHASE(diversa)/SCATTER(uguale)->variabile: scatterLocation[diversa];] -> movToLocation(location)[uguale];
firstSpawn(); -> variabili: posizioneSpawn[diversa],priority[diversa],limitPoint[diversa],maxSecondFood[uguale];
pacmanRespawnSpawn(); -> ;

*/

#include "Ghost.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGhost::AGhost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Sets default values
AGhost::AGhost(int mapLevel, APacman* pacman)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->mapLevel = mapLevel;
	this->pacman = pacman;
}

void AGhost::NotifyActorBeginOverlap(AActor* OtherActor) {


	if (OtherActor && OtherActor->IsA(APacman::StaticClass())) // Check if the other actor is of the desired class
	{
		UE_LOG(LogTemp, Display, TEXT("GHOST[%s] COLPITO DA PACMAN"), *GetName());
		if (movmentState == Util::FRIGHTENED && isDead==false) {
			isDead = true;
			speed = 10;
			MeshComp->SetMaterial(0, deadMaterial);
			MeshComp2->SetMaterial(0, deadMaterial);
			*contatoreFantasmiMangiati= *contatoreFantasmiMangiati +1;
			if (*contatoreFantasmiMangiati == 1)
				*points = *points + 200;
			if (*contatoreFantasmiMangiati == 2)
				*points = *points + 400;
			if (*contatoreFantasmiMangiati == 3)
				*points = *points + 800;
			if (*contatoreFantasmiMangiati == 4)
				*points = *points + 1600;
			//UE_LOG(LogTemp, Display, TEXT("Pacman ha mangiato il [%s] un fantsma ora i suoi punti sono: %s"), *FString::FromInt(*contatoreFantasmiMangiati), *FString::FromInt(*points));
			return;
		}

		if (movmentState != Util::FRIGHTENED && isDead == false) {

			pacman->isDead = true;
			pacman->SetActorHiddenInGame(true);
		}
	}

}


// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();

	for (UActorComponent* Component : GetComponents())
	{
		// Check if the component is a StaticMeshComponent
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
		if (StaticMeshComponent)
		{
			if (StaticMeshComponent->GetName() == "Body") {
				MeshComp = StaticMeshComponent;
			}
			if (StaticMeshComponent->GetName() == "Head") {
				MeshComp2 = StaticMeshComponent;
			}
		}
	}



	defaultMaterial = MeshComp->GetMaterial(0);

	srand(123456789);

	speed = 5 * levelGhostSpeed[mapLevel-1];
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("GHOST[%s] tick movement delta: %f"), *GetName(), DeltaTime);

	//Controll if ghost is active (activate when game start)
	if (!isActive)
		return;


	//Check frightnedmode
	if (pacman->isFrightnedModeOn && movmentState!=Util::FRIGHTENED) {
		movmentState = Util::FRIGHTENED;
		speed = 5 * levelGhostFrightnedSpeed[mapLevel - 1];
		frightnedTimer = levelFrightnedTime[mapLevel - 1];
		//change material
		MeshComp->SetMaterial(0, frightnedMaterial);
		MeshComp2->SetMaterial(0, frightnedMaterial);
	}


	float flashTimes=5;

	if (levelFrightnedTime[mapLevel - 1] == 0) {
		flashTimes = 0;
	}
	else if (levelFrightnedTime[mapLevel - 1] == 1) {
		flashTimes = 3;
	}



	if (frightnedTimer < flashTimes*0.3 && !isDead) {
		flashIndicatorTime += DeltaTime;
		if (flashIndicatorTime > 0.15) {
			//Accendere fantsma
			MeshComp->SetMaterial(0, flashFrightnedMaterial);
			MeshComp2->SetMaterial(0, flashFrightnedMaterial);

		}
		if (flashIndicatorTime > 0.3) {
			//Spegnere fantasma
			MeshComp->SetMaterial(0, frightnedMaterial);
			MeshComp2->SetMaterial(0, frightnedMaterial);
			flashIndicatorTime = 0;
		}
	}

	

	//Frightned timer
	if (frightnedTimer <= 0) {
		frightnedTimer = 0;
		speed = 5 * levelGhostSpeed[mapLevel - 1];
		pacman->isFrightnedModeOn = false;
		*contatoreFantasmiMangiati = 0;
		movmentState = oldMovementState;
		MeshComp->SetMaterial(0, defaultMaterial);
		MeshComp2->SetMaterial(0, defaultMaterial);
	}

	//Frightned timer
	if (frightnedTimer > 0 && !isDead) {
		frightnedTimer -= DeltaTime;
	}



	//Movment
	//CALCULATE MOVEMENT DIRECTION VECTOR
	FVector MovementVector;
	switch (movState)
	{
	case Util::LEFT: MovementVector = FVector(-1.f, 0.f, 0.f);
		break;
	case Util::RIGHT: MovementVector = FVector(1.f, 0.f, 0.f);
		break;
	case Util::UP: MovementVector = FVector(0.f, -1.f, 0.f);
		break;
	case Util::DOWN: MovementVector = FVector(0.f, 1.f, 0.f);
		break;
	default:
		break;
	}
	//PERFORM MOVEMENT
	FVector NewLocation = GetActorLocation() + (MovementVector * speed);
	SetActorLocation(NewLocation);


	if (ForcedRouteIndex != -1) {
		ForcedMovement((MovementVector * speed));
		return;
	}


	if (movmentState != Util::FRIGHTENED) {
		//Time managmen
		timeIndicator += DeltaTime;

		// TODO: CAMBIARE IN 1/60
		if (timeIndicator >= 0.017) {
			secondsPassed += timeIndicator;
			timeIndicator = 0;
			//if (movmentState == Util::CHASE)
				//UE_LOG(LogTemp, Warning, TEXT("[%s] CHASE per il fantasma: %s"), *FString::SanitizeFloat(secondsPassed), *GetName());
			//if (movmentState == Util::SCATTER)
				//UE_LOG(LogTemp, Warning, TEXT("[%s] SCATTER per il fantasma: %s"), *FString::SanitizeFloat(secondsPassed), *GetName());
			//UE_LOG(LogTemp, Warning, TEXT("[%s] Second passed: %s"), *GetName(), *FString::SanitizeFloat(secondsPassed / 100));
		}

		if (!invertMovementFlag && movmentCycleIndicator != 7) {


			int mapIndicator = 0;
			if (mapLevel == 1) {
				mapIndicator = 0;
			}

			if (mapLevel >= 2 && mapLevel <= 4) {
				mapIndicator = 1;
			}

			if (mapLevel >= 5) {
				mapIndicator = 2;
			}


			if (secondsPassed >= levelMovmentModeTABLE[mapIndicator][movmentCycleIndicator]) {
				//UE_LOG(LogTemp, Warning, TEXT("[%s] ha cambiato modalità"), *GetName());
				movmentCycleIndicator++;
				if (movmentCycleIndicator % 2 == 0) {
					movmentState = Util::SCATTER;
					oldMovementState = Util::SCATTER;
					//pari = Scatter
				}
				else {
					//disapri = Chase
					movmentState = Util::CHASE;
					oldMovementState = Util::CHASE;
				}
				invertMovementFlag = true;
				secondsPassed = 0;
			}
		}
	}

	ACustom_MapBlock* destinationBlock = getNextBlock(currentBlock, movState);
	
	FVector underBlockCenterLoc = destinationBlock->GetActorLocation();
	FVector ghostLoc = GetActorLocation();


	HELP+=1;
	bool test = IsBetween(ghostLoc, underBlockCenterLoc - (MovementVector * speed) / 2, underBlockCenterLoc + (MovementVector * speed) / 2);
	if (!test && HELP> 300) {
		//UE_LOG(LogTemp, Warning, TEXT("IS BETWEEN e FALSO"));
	}
	//CHECK IF GHOST HAS REACHED THE CENTER OF THE BLOCK
	if (IsBetween(ghostLoc, underBlockCenterLoc - (MovementVector * speed)/2, underBlockCenterLoc + (MovementVector * speed)/2)) {
		
		if (isDead && destinationBlock== defaultExitCageBlock) {
			isDead = false;
			frightnedTimer = 0;
			speed = 5 * levelGhostSpeed[mapLevel - 1];
			MeshComp->SetMaterial(0, defaultMaterial);
			MeshComp2->SetMaterial(0, defaultMaterial);
		}

		//Check for teleport, if yes teleport
		if (destinationBlock->isTeleport && destinationBlock->direction == movState) {
			FString output;
			switch (movState)
			{
			case Util::IDLE: output = "IDLE";
				break;
			case Util::LEFT: output = "LEFT";
				break;
			case Util::RIGHT: output = "RIGHT";
				break;
			case Util::UP: output = "UP";
				break;
			case Util::DOWN: output = "DOWN";
				break;
			default: output = "Unknown";
			}

			FString output2;
			switch (destinationBlock->direction)
			{
			case Util::IDLE: output2 = "IDLE";
				break;
			case Util::LEFT: output2 = "LEFT";
				break;
			case Util::RIGHT: output2 = "RIGHT";
				break;
			case Util::UP: output2 = "UP";
				break;
			case Util::DOWN: output2 = "DOWN";
				break;
			default: output2 = "Unknown";
			}

			UE_LOG(LogTemp, Warning, TEXT("Movstate: [%s] | TeleportNeeded: [%s]"),*output,*output2);

			teleportGhost(destinationBlock);
			return;
		}

		//AGGIORNARE LA POSIZIONE
		FVector BlockLocation = destinationBlock->GetActorLocation();
		//fix position to center currentblock
		FVector CurrentLocation = GetActorLocation();
		FVector FixedLocation = FVector(BlockLocation.X, BlockLocation.Y, CurrentLocation.Z);
		SetActorLocation(FixedLocation);

		ACustom_MapBlock* OldCurrentBlockTEMP = currentBlock;

		currentBlock = destinationBlock;

		FString beforeStringMove;

		switch (movState)
		{
			case Util::IDLE: beforeStringMove =  "IDLE";
				break;
			case Util::LEFT: beforeStringMove = "LEFT";
				break;
			case Util::RIGHT: beforeStringMove = "RIGHT";
				break;
			case Util::UP: beforeStringMove = "UP";
				break;
			case Util::DOWN: beforeStringMove = "DOWN";
				break;
			default: beforeStringMove = "Unknown";
		}

		FString nextStringMove;

		switch (nextMove)
		{
			case Util::IDLE: nextStringMove = "IDLE";
				break;
			case Util::LEFT: nextStringMove = "LEFT";
				break;
			case Util::RIGHT: nextStringMove = "RIGHT";
				break;
			case Util::UP: nextStringMove = "UP";
				break;
			case Util::DOWN: nextStringMove = "DOWN";
				break;
			default: nextStringMove = "Unknown";
		}


		Util::MovementState OldMoveStateTEMP = movState;
		
		if (movmentState == Util::FRIGHTENED && !isDead) {

			

			movState = next_move_frightned();


		}
		else {
			movState = nextMove;
		}
		

		if (invertMovementFlag) {

			switch (OldMoveStateTEMP)
			{
			case Util::IDLE:
				break;
			case Util::LEFT:
				movState = Util::RIGHT;
				break;
			case Util::RIGHT:
				movState = Util::LEFT;
				break;
			case Util::UP:
				movState = Util::DOWN;
				break;
			case Util::DOWN:
				movState = Util::UP;
				break;
			}

			//currentBlock = getNextBlock(OldCurrentBlockTEMP, movState);

			invertMovementFlag = false;
		}

		nextMove = next_move();

		//UE_LOG(LogTemp, Warning, TEXT("IL FANTASMA HA RAGGIUNTO IL BLOCCO in [%s] ticks, block name [%s], Before Move [%s], Next Move [%s]"), *FString::FromInt(HELP), *destinationBlock->GetFName().ToString(), *beforeStringMove, *nextStringMove);
		HELP = 0;
	}


}


// This class will activate all the ghost functions
void AGhost::Activate() {
	isActive = true;
}

// This class will deactivate all the ghost functions
void AGhost::Deactivate() {
	isActive = false;
}

void AGhost::check_for_target_change() {

}

Util::MovementState AGhost::next_move(){



	ACustom_MapBlock* nextBlock = getNextBlock(currentBlock, movState);

	ACustom_MapBlock* left = getNextBlock(nextBlock, Util::LEFT);
	ACustom_MapBlock* right = getNextBlock(nextBlock, Util::RIGHT);
	ACustom_MapBlock* up = getNextBlock(nextBlock, Util::UP);
	ACustom_MapBlock* down = getNextBlock(nextBlock, Util::DOWN);

	switch (movState)
	{
	case Util::IDLE:
		break;
	case Util::LEFT:
		right = nullptr;
		break;
	case Util::RIGHT:
		left = nullptr;
		break;
	case Util::UP:
		down = nullptr;
		break;
	case Util::DOWN:
		up = nullptr;
		break;
	}

	ACustom_MapBlock* dir[4] = {right, down, left, up};
	float min_dist = 0;

	

	FVector target_pos;
	if (movmentState==Util::CHASE) {
		check_for_target_change();
		target_pos  = target->getCenterPoint();
	}
	else {
		target_pos = scatterBlock->getCenterPoint();
	}



	if (isDead) {
		target_pos = defaultExitCageBlock->getCenterPoint();
	}
	//UE_LOG(LogTemp, Display, TEXT("TARGET OF GHOST[%s]: (%f, %f, %f)"), *GetName(), target_pos.X, target_pos.Y, target_pos.Z);




	int c=10;
	Util::MovementState dirTwin[4] = {Util::RIGHT, Util::DOWN, Util::LEFT, Util::UP};
	for (int i = 0; i < 4; i++)
	{
		if (dir[i] == nullptr)
			continue;
		if (!dir[i]->isWalkable)
			continue;
		FVector pos_block = dir[i]->getCenterPoint();
		float temp = FVector::Distance(target_pos, pos_block);
		if (temp <= min_dist || min_dist == 0) {
			min_dist = temp;
			c = i;
		}
	}
	if (c == 10) {
		//UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [next move]"));
		return Util::IDLE;
	}
	//PRIORITY in alto, a sinistra, in basso, a destra
	return dirTwin[c];

}

Util::MovementState AGhost::next_move_frightned() {

	ACustom_MapBlock* left = getNextBlock(currentBlock, Util::LEFT);
	ACustom_MapBlock* right = getNextBlock(currentBlock, Util::RIGHT);
	ACustom_MapBlock* up = getNextBlock(currentBlock, Util::UP);
	ACustom_MapBlock* down = getNextBlock(currentBlock, Util::DOWN);

	switch (movState)
	{
	case Util::IDLE:
		break;
	case Util::LEFT:
		right = nullptr;
		break;
	case Util::RIGHT:
		left = nullptr;
		break;
	case Util::UP:
		down = nullptr;
		break;
	case Util::DOWN:
		up = nullptr;
		break;
	}

	ACustom_MapBlock* dir[4] = { right, down, left, up };


	int rand_val = rand(); // genera un valore casuale con rand()
	int last_two_bits = (rand_val & 0b11); // maschera gli ultimi 2 bit
	int value = (last_two_bits % 4); // converte in un valore tra 1 e 4

	int c = 10;
	Util::MovementState dirTwin[4] = { Util::RIGHT, Util::DOWN, Util::LEFT, Util::UP };
	for (int i = value; i < 4; i++)
	{
		if (dir[i] == nullptr)
			continue;
		if (!dir[i]->isWalkable)
			continue;
		
		c = i;
		return dirTwin[c];
	}
	for (int i = 0 ; i < value; i++)
	{
		if (dir[i] == nullptr)
			continue;
		if (!dir[i]->isWalkable)
			continue;

		c = i;
		return dirTwin[c];
	}
	if (c == 10) {
		//UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [next move]"));
		return Util::IDLE;
	}


	//PRIORITY in alto, a sinistra, in basso, a destra
	return dirTwin[c];

}

ACustom_MapBlock* AGhost::getNextBlock(ACustom_MapBlock* block, Util::MovementState movState_) {
	if (block == nullptr) {
		return nullptr;
	}

	switch (movState_)
	{
		case Util::IDLE: return block;
		case Util::UP: return block->sideBlocks[0];
		case Util::LEFT: return block->sideBlocks[1];
		case Util::DOWN: return block->sideBlocks[2];
		case Util::RIGHT: return block->sideBlocks[3];
		default: {
			//UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA: NULLPOINTER RITORNATO"));
			return nullptr;
		}
	}
}

bool AGhost::IsBetween(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB)
{
	// Find the minimum and maximum X and Y values
	float MinX = FMath::Min(VectorA.X, VectorB.X);
	float MaxX = FMath::Max(VectorA.X, VectorB.X);
	float MinY = FMath::Min(VectorA.Y, VectorB.Y);
	float MaxY = FMath::Max(VectorA.Y, VectorB.Y);

	// Check if the X and Y components of VectorToCheck are within the range of VectorA and VectorB
	bool IsBetweenX = VectorToCheck.X >= MinX && VectorToCheck.X <= MaxX;
	bool IsBetweenY = VectorToCheck.Y >= MinY && VectorToCheck.Y <= MaxY;

	return IsBetweenX && IsBetweenY;
}


void AGhost::ForcedMovement(FVector movementVelocityVector) {

	if (isInCage==true) {
		FVector CurrentDestination = ForcedRoute[ForcedRouteIndex];
		FVector ghostLoc = GetActorLocation();
		//Check if destination block is reached
		if (IsBetween(ghostLoc, CurrentDestination - movementVelocityVector/2, CurrentDestination + movementVelocityVector / 2)) {

			//fix position to center currentblock
			FVector CurrentLocation = GetActorLocation();
			FVector FixedLocation = FVector(CurrentDestination.X, CurrentDestination.Y, CurrentLocation.Z);
			SetActorLocation(FixedLocation);

			// Aggiornare ForcedIndex
			ForcedRouteIndex++;
			if (ForcedRouteIndex == 4)ForcedRouteIndex = 0;

			movState = ForcedRouteMovement[ForcedRouteIndex];
		}
	}else {
		if (isInCageOld != isInCage) {
			//UE_LOG(LogTemp, Warning, TEXT("IL FANTASMA STA EFFETTUANDO IL CICLO DI USCITA"));
			ForcedRouteIndex = 0;
		}

		FVector CurrentDestination = ExitRoute[ForcedRouteIndex];
		FVector ghostLoc = GetActorLocation();

		if (isInCageOld != isInCage) {
			if (ghostLoc.Y <= -100) {
				UE_LOG(LogTemp, Warning, TEXT("DOWN"));
				ForcedExitMovement[0] = Util::DOWN;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("UP"));
				ForcedExitMovement[0] = Util::UP;
			}
			movState = ForcedExitMovement[0];
		}

		//Check if destination block is reached
		if (IsBetween(ghostLoc, CurrentDestination - movementVelocityVector / 2, CurrentDestination + movementVelocityVector / 2)) {

			//fix position to center currentblock
			FVector CurrentLocation = GetActorLocation();
			FVector FixedLocation = FVector(CurrentDestination.X, CurrentDestination.Y, CurrentLocation.Z);
			SetActorLocation(FixedLocation);

			// Aggiornare ForcedIndex
			ForcedRouteIndex++;
			if (ForcedRouteIndex == 5) {
				ForcedRouteIndex = -1;
				movState = Util::LEFT;
				currentBlock = defaultExitCageBlock;
				return;
			}

			movState = ForcedExitMovement[ForcedRouteIndex];
		}


	}
	isInCageOld = isInCage;
}

void AGhost::teleportGhost(ACustom_MapBlock* teleportBlock) {
	SetActorLocation(teleportBlock->teleportDestination->getCenterPoint());
	currentBlock = teleportBlock->teleportDestination;
}


