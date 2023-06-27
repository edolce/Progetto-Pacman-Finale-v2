// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde_Ghost.h"

// Sets default values
AClyde_Ghost::AClyde_Ghost()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AClyde_Ghost::check_for_target_change() {

	ACustom_MapBlock* currentPacman = Super::pacman->currentBlock;

	//calc distance pacman ghost
	float temp = FVector::Distance(currentPacman->GetActorLocation(), currentBlock->GetActorLocation());
	

	if (temp >= 800) {
		if (Super::pacman->nextBlock != nullptr) {
			Super::target = Super::pacman->nextBlock;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [INSIDE BLINKY-Check fdor next target]"));
			Super::target = Super::pacman->currentBlock;
		}
	}
	else {
		Super::target = Super::scatterBlock;
	}


	
	
	





	
}