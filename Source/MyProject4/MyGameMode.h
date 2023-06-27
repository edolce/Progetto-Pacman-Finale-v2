// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CustomMap.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameMode.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT4_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AMyGameMode();

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACustomMap> customMap;

	void BeginPlay();
	void Tick(float DeltaTime);


	bool isLevlFinished=false;

	ACustomMap* currentMap;

};
