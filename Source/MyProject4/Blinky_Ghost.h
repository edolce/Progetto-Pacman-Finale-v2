// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghost.h"
#include "Blinky_Ghost.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API ABlinky_Ghost : public AGhost
{
	GENERATED_BODY()

public:
	ABlinky_Ghost();
	virtual void check_for_target_change() override;

};
