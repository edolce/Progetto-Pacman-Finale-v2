// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ghost.h"
#include "Clyde_Ghost.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API AClyde_Ghost : public AGhost
{
	GENERATED_BODY()
public:
	AClyde_Ghost();
	virtual void check_for_target_change() override;
};
