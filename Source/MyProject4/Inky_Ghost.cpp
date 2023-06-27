// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky_Ghost.h"
#include "Ghost.h"

// Sets default values
AInky_Ghost::AInky_Ghost()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}









void AInky_Ghost::check_for_target_change() {

	ACustom_MapBlock* current = Super::pacman->currentBlock;
	ACustom_MapBlock* pacmanOffsetBlock;

	Util::MovementState movementToCheck = Super::pacman->movState;

	if (movementToCheck == Util::IDLE) {
		movementToCheck = Super::pacman->permanentOldState;
	}

	switch (movementToCheck)
	{
	case Util::IDLE:
		UE_LOG(LogTemp, Warning, TEXT("IDLEEEEEEEEEE"));
		pacmanOffsetBlock = current;
		break;
	case Util::UP:
		pacmanOffsetBlock = get2BlockOffset(get2BlockOffset(current, Util::UP), Util::LEFT);
		break;
	case Util::LEFT:
		pacmanOffsetBlock = get2BlockOffset(current, Util::LEFT);
		break;
	case Util::DOWN:
		pacmanOffsetBlock = get2BlockOffset(current, Util::DOWN);
		break;
	case Util::RIGHT:
		pacmanOffsetBlock = get2BlockOffset(current, Util::RIGHT);
		break;
	default:
		pacmanOffsetBlock = nullptr;
	}

	ACustom_MapBlock*  newTarget = getDistance(pacmanOffsetBlock, blinky->currentBlock);

	if (newTarget != nullptr) {
		Super::target = newTarget;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [INKY]"));
		Super::target = Super::pacman->currentBlock;
	}
}










ACustom_MapBlock* AInky_Ghost::get2BlockOffset(ACustom_MapBlock* block, Util::MovementState mov) {

	ACustom_MapBlock* finalOffset = Super::getNextBlock(block, mov);
	ACustom_MapBlock* tempOffset;

	if (finalOffset == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("ERRORE TARGET NULL NEL CALCOLO DEL OFSET DI 4 BLOCCHI"));
	}

	tempOffset = Super::getNextBlock(finalOffset, mov);
	if (tempOffset == nullptr) {
		return finalOffset;
	}
	finalOffset = tempOffset;

	return finalOffset;
}


ACustom_MapBlock* AInky_Ghost::getDistance(ACustom_MapBlock* pacmanOffsetBlock, ACustom_MapBlock* blinkyBlock) {

	int xFinal = blinkyBlock->x - (blinkyBlock->x - pacmanOffsetBlock->x)*2;

	int yFinal = blinkyBlock->y - (blinkyBlock->y - pacmanOffsetBlock->y)*2;

	if (yFinal < 0) {
		yFinal = 0;
	}

	if (yFinal>35) {
		yFinal = 35;
	}

	if (xFinal < 0) {
		xFinal = 0;
	}

	if (xFinal>27) {
		xFinal = 27;
	}

	return blocksMap[yFinal][xFinal];
}