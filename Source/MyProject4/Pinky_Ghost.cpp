// Fill out your copyright notice in the Description page of Project Settings.

#include "Pinky_Ghost.h"
#include "Ghost.h"

// Sets default values
APinky_Ghost::APinky_Ghost()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APinky_Ghost::check_for_target_change() {

	ACustom_MapBlock* current = Super::pacman->currentBlock;
	ACustom_MapBlock* newTarget;

	Util::MovementState movementToCheck = Super::pacman->movState;
		
	if (movementToCheck== Util::IDLE) {
		movementToCheck = Super::pacman->permanentOldState;
	}

	switch (movementToCheck)
	{
	case Util::IDLE:
		UE_LOG(LogTemp, Warning, TEXT("IDLEEEEEEEEEE"));
		newTarget = current;
		break;
	case Util::UP:
		newTarget = get4BlockOffset(get4BlockOffset(current, Util::UP), Util::LEFT);
		break;
	case Util::LEFT:
		newTarget = get4BlockOffset(current, Util::LEFT);
		break;
	case Util::DOWN:
		newTarget = get4BlockOffset(current, Util::DOWN);
		break;
	case Util::RIGHT:
		newTarget = get4BlockOffset(current, Util::RIGHT);
		break;
	default: 
		newTarget = nullptr;
	}


	if (newTarget != nullptr) {
		Super::target = newTarget;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [PINKY]"));
		Super::target = Super::pacman->currentBlock;
	}
}


ACustom_MapBlock* APinky_Ghost::get4BlockOffset(ACustom_MapBlock* block, Util::MovementState mov) {

	ACustom_MapBlock* finalOffset = Super::getNextBlock(block, mov);
	ACustom_MapBlock* tempOffset;

	if (finalOffset == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("ERRORE TARGET NULL NEL CALCOLO DEL OFSET DI 4 BLOCCHI"));
		return block;
	}
		
	tempOffset = Super::getNextBlock(finalOffset, mov);
	if(tempOffset==nullptr){
		return finalOffset;
	}
	finalOffset = tempOffset;

	tempOffset = Super::getNextBlock(finalOffset, mov);
	if (tempOffset == nullptr) {
		return finalOffset;
	}
	finalOffset = tempOffset;

	tempOffset = Super::getNextBlock(finalOffset, mov);
	if (tempOffset == nullptr) {
		return finalOffset;
	}
	finalOffset = tempOffset;

	return finalOffset;
}
