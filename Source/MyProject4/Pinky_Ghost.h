// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghost.h"
#include "Pinky_Ghost.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API APinky_Ghost : public AGhost
{
	GENERATED_BODY()

public:
	APinky_Ghost();
	virtual void check_for_target_change() override;
	ACustom_MapBlock* get4BlockOffset(ACustom_MapBlock* block,Util::MovementState mov);
};
