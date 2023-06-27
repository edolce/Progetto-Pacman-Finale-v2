// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghost.h"
#include "Blinky_Ghost.h"
#include "Inky_Ghost.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API AInky_Ghost : public AGhost
{
	GENERATED_BODY()

public:
	AInky_Ghost();
	virtual void check_for_target_change() override;
	ACustom_MapBlock* get2BlockOffset(ACustom_MapBlock* block, Util::MovementState mov);

	ABlinky_Ghost* blinky;
	ACustom_MapBlock* getDistance(ACustom_MapBlock* pacmanOffsetBlock, ACustom_MapBlock* blinkyBlock);

	ACustom_MapBlock* blocksMap[36][28];
};
