// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MYPROJECT4_API Util
{
public:
	Util();
	~Util();
	enum MovementState {
		IDLE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum GhostState {
		CHASE,
		SCATTER,
		FRIGHTENED
	};

	enum FoodType {
		NORMAL,
		SUPER,
		CHERRY,
		STRAWBERRY,
		PEACH,
		APPLE,
		GRAPES,
		GALAXIAN,
		BELL,
		KEY
	};
};
