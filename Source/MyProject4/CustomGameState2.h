// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CustomGameState2.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API ACustomGameState2 : public AGameState
{
	GENERATED_BODY()

public:
	ACustomGameState2();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
		float ElapsedTime_;
	


private:
    float GameStartTime;
};
