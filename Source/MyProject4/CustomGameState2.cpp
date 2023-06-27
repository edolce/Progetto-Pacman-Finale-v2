// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState2.h"


ACustomGameState2::ACustomGameState2()
{
    PrimaryActorTick.bCanEverTick = true;
    ElapsedTime_ = 0.0f;
    GameStartTime = 0.0f;
}



void ACustomGameState2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GameStartTime == 0.0f)
    {
        GameStartTime = GetWorld()->GetTimeSeconds();
    }

    ElapsedTime_ = GetWorld()->GetTimeSeconds() - GameStartTime;

}