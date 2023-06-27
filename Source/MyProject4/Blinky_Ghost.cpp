// Fill out your copyright notice in the Description page of Project Settings.

#include "Blinky_Ghost.h"
#include "Ghost.h"


// Sets default values
ABlinky_Ghost::ABlinky_Ghost()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABlinky_Ghost::check_for_target_change() {
	if (Super::pacman->nextBlock != nullptr) {
		Super::target = Super::pacman->nextBlock;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ERRORE NELLA SCELTA DEL MOVIMENTO DEL FANTASMA [INSIDE BLINKY-Check fdor next target]"));
		Super::target = Super::pacman->currentBlock;
	}
}