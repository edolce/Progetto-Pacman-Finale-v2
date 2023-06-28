// Fill out your copyright notice in the Description page of Project Settings.



#include "Pacman.h"
#include "Kismet/GameplayStatics.h"
#include "CustomFood.h"

// Sets default values
APacman::APacman()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APacman::BeginPlay()
{

	Super::BeginPlay();

}


  

void APacman::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACustom_MapBlock::StaticClass())) // Check if the other actor is of the desired class
	{
		// Handle the collision event here
		// For example, print a message to the console
		//UE_LOG(LogTemp, Warning, TEXT("Pawn collided with OtherActor"));
	}
}

void APacman::NotifyActorBeginOverlap(AActor* OtherActor) {
	//UE_LOG(LogTemp, Warning, TEXT("Pacman ha toccato la pallina"));

	


	if (OtherActor && OtherActor->IsA(ACustomFood::StaticClass())) {
		ACustomFood* food = dynamic_cast<ACustomFood*>(OtherActor);

		if (food) {
			skipMovment = true;
			skipMovmentTimer = 0.017;
		}

		OtherActor->Destroy();
		switch (food->type) {
		case Util::NORMAL:
			point+=10;
			rawPoints++;
			(*PointsForCompletition)--;

			if (*PointsForCompletition == 0) {
				*isMapCompleted = true;
			}

			break;
		case Util::SUPER:
			point += 100;
			(*PointsForCompletition)--;
			//TURN OF FRIGHTNED MODE
			isFrightnedModeOn = true;
			if (*PointsForCompletition == 0) {
				*isMapCompleted = true;
			}
			break;

		case Util::CHERRY:
			point += 100;
			break;
		case Util::STRAWBERRY:
			point += 300;
			break;
		case Util::PEACH:
			point += 500;
			break;
		case Util::APPLE:
			point += 700;
			break;
		case Util::GRAPES:
			point += 1000;
			break;
		case Util::GALAXIAN:
			point += 2000;
			break;
		case Util::BELL:
			point += 3000;
			break;
		case Util::KEY:
			point += 5000;
			break;
		default:
			break;
		}
		return;
	}
}









// Called every frame
void APacman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (skipMovment) {
		if(skipMovmentTimer<=0){
			skipMovment = false;
			skipMovmentTimer = 0;
		}
		else {
			skipMovmentTimer -= DeltaTime;
			return;
		}
	}

	//IF IDLE NO MOVEMENT
	if (movState == Util::IDLE) {
		if(bufferedMove == Util::IDLE) return;
		FirstMoveFlag = true;
		movState = bufferedMove;
	}


	//Inizializzazione del nextblock
	if (currentBlock == nextBlock || nextBlock == nullptr)
	{
		nextBlock = getNextBlock();
	}

	//Check for teleport, if yes teleport
	if (currentBlock->isTeleport && currentBlock->direction==movState) {
		if (currentBlock != nullptr) teleportPacman(currentBlock);
		else UE_LOG(LogTemp, Warning, TEXT("ERROR"));
	}

	//
	if (!(nextBlock->isWalkable)) {
		//fix position
		FVector BlockLocation = currentBlock->GetActorLocation();
		//fix position to center currentblock
		FVector CurrentLocation = GetActorLocation();
		FVector FixedLocation = FVector(BlockLocation.X, BlockLocation.Y, CurrentLocation.Z);
		SetActorLocation(FixedLocation);

		movState = Util::IDLE;
		bufferedMove = Util::IDLE;
		nextBlock = nullptr;
	}

	

	//IF NOT IDLE PERFORM MOVEMENT

	//CALCULATE MOVEMENT DIRECTION VECTOR
	FVector MovementVector;
	switch (movState)
	{
	case Util::LEFT: MovementVector = FVector(-1.f, 0.f, 0.f);
		break;
	case Util::RIGHT: MovementVector = FVector(1.f, 0.f, 0.f);
		break;
	case Util::UP: MovementVector = FVector(0.f, -1.f, 0.f);
		break;
	case Util::DOWN: MovementVector = FVector(0.f, 1.f, 0.f);
		break;
	default:
		break;
	}

	if (isFrightnedModeOn) {
		speed = 5 * levelPacmanFrightnedSpeed[mapLevel - 1];
	}
	else {
		speed = 5 * levelPacmanSpeed[mapLevel - 1];
	}

	//PERFORM MOVEMENT
	FVector NewLocation = GetActorLocation() + (MovementVector * speed);
	if (nextBlock != nullptr) {
		//FString ActorLocationString = FString::Printf(TEXT("Vector: %f %f %f"), currentBlock->GetActorLocation().X, currentBlock->GetActorLocation().Y, currentBlock->GetActorLocation().Z);
		//FString NewLocationString = FString::Printf(TEXT("Vector: %f %f %f"), nextBlock->GetActorLocation().X, nextBlock->GetActorLocation().Y, nextBlock->GetActorLocation().Z);
		//UE_LOG(LogTemp, Warning, TEXT("CURRENT:%s NEXT:%s"), *ActorLocationString, *NewLocationString);

	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("ERRORE NULLPOINTER NEXTMOVE"));
	}
	SetActorLocation(NewLocation);


	//if is not idle, check if has reached the center of block (check if )
	if (movState != Util::IDLE) {
		FVector underBlockCenterLoc = nextBlock->getCenterPoint();
		FVector pacmanLoc = GetActorLocation();

		//UE_LOG(LogTemp, Display, TEXT("%s: (%f, %f, %f), %s: (%f, %f, %f)"), *GetName(),
			//pacmanLoc.X, pacmanLoc.Y, pacmanLoc.Z,
			//*currentBlock->GetName(),
			//underBlockCenterLoc.X, underBlockCenterLoc.Y, underBlockCenterLoc.Z);


		//CHECK IF PACKMAN HAS REACHED THE CENTER OF THE BLOCK
		if (IsBetween(pacmanLoc,underBlockCenterLoc- (MovementVector * speed)/2,underBlockCenterLoc+ (MovementVector * speed)/2)) {
			//UE_LOG(LogTemp, Warning, TEXT("OLD:%s REACHED:%s"), *currentBlock->GetFName().ToString(), *nextBlock->GetFName().ToString());

			
			//SAME LOC, update currentBLock
			
			

			//CHECK IF ITS THERES A TURN (IF YES CHECK FOR BUFFERED MOVMENT (CHECK IF VALIS, IF YES PERFORM TURN movState=bufMove))
			if (movState != bufferedMove) {
				if (isTurnPossible(bufferedMove)) {
					FVector BlockLocation = nextBlock->GetActorLocation();
					//fix position to center currentblock
					FVector CurrentLocation = GetActorLocation();
					FVector FixedLocation = FVector(BlockLocation.X, BlockLocation.Y, CurrentLocation.Z);
					SetActorLocation(FixedLocation);
					//perform turn
					movState = bufferedMove;
				}
			}

			currentBlock = nextBlock;

			//CHECK IF NEXT BLOCK IN LINE IS WALKABLE (IF yes continue/IF not stop and change pos to IDLE)
		}
	}

	if (oldState != movState) {
		permanentOldState = oldState;
	}
	oldState = movState;
}



bool APacman::isTurnPossible(Util::MovementState movement) {
	//claculate next pos
	FVector nextPos = nextBlock->getCenterPoint();

	switch (movement)
	{
	case Util::IDLE: return nullptr;
		break;
	case Util::LEFT:
		nextPos.X -= 100;
		break;
	case Util::RIGHT:
		nextPos.X += 100;
		break;
	case Util::UP:
		nextPos.Y -= 100;
		break;
	case Util::DOWN:
		nextPos.Y += 100;
		break;
	default:
		break;
	}

	// Get the closest AActor to the TargetLocation
	AActor* ClosestActor = getClosestActor(nextPos);
	//UE_LOG(LogTemp, Warning, TEXT("TURN FOUND ON OBJECT: %s"), *ClosestActor->GetFName().ToString());
	return dynamic_cast<ACustom_MapBlock*>(ClosestActor)->isWalkable;
}



// Called to bind functionality to input
void APacman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LR_Movement"), this, &APacman::MoveLR);
	PlayerInputComponent->BindAxis(TEXT("UD_Movement"), this, &APacman::MoveUD);

}


void APacman::MoveLR(float movementDelta) {
	if (movementDelta == 1) {
		bufferedMove = Util::RIGHT;
	}
	if (movementDelta == -1) {
		bufferedMove = Util::LEFT;
	}
}

void APacman::MoveUD(float movementDelta){
	if (movementDelta == 1) {
		bufferedMove = Util::DOWN;
	}
	if (movementDelta == -1) {
		bufferedMove = Util::UP;
	}
}



//util
bool APacman::isBlockWalkable(ACustom_MapBlock& block) {
	return block.isWalkable;
}

ACustom_MapBlock* APacman::getNextBlock() {
	//claculate next pos
	FVector nextPos = currentBlock->getCenterPoint();

	switch (movState)
	{
	case Util::IDLE: return nullptr;
		break;
	case Util::LEFT:
		nextPos.X -= 100;
		break;
	case Util::RIGHT:
		nextPos.X += 100;
		break;
	case Util::UP:
		nextPos.Y -= 100;
		break;
	case Util::DOWN:
		nextPos.Y += 100;
		break;
	default:
		break;
	}

	// Get the closest AActor to the TargetLocation
	AActor* ClosestActor = getClosestActor(nextPos);
	return dynamic_cast<ACustom_MapBlock*>(ClosestActor);
}


AActor* APacman::getClosestActor(const FVector& TargetLocation)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustom_MapBlock::StaticClass(), Actors);

	ACustom_MapBlock* ClosestActor = nullptr;
	float ClosestDistanceSq = MAX_FLT;

	for (AActor* Actor : Actors)
	{
		const float DistanceSq = (dynamic_cast<ACustom_MapBlock*>(Actor)->getCenterPoint() - TargetLocation).SizeSquared();

		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestActor = dynamic_cast<ACustom_MapBlock*>(Actor);
			ClosestDistanceSq = DistanceSq;
		}
	}

	if (ClosestActor != nullptr)
	{
		return ClosestActor;
	}
	return nullptr;
}

bool APacman::IsBetween(const FVector& VectorToCheck, const FVector& VectorA, const FVector& VectorB)
{
	// Find the minimum and maximum X and Y values
	float MinX = FMath::Min(VectorA.X, VectorB.X);
	float MaxX = FMath::Max(VectorA.X, VectorB.X);
	float MinY = FMath::Min(VectorA.Y, VectorB.Y);
	float MaxY = FMath::Max(VectorA.Y, VectorB.Y);
	 
	// Check if the X and Y components of VectorToCheck are within the range of VectorA and VectorB
	bool IsBetweenX = VectorToCheck.X >= MinX && VectorToCheck.X <= MaxX;
	bool IsBetweenY = VectorToCheck.Y >= MinY && VectorToCheck.Y <= MaxY;

	return IsBetweenX && IsBetweenY;
}

void APacman::teleportPacman(ACustom_MapBlock* teleportBlock) {
	int Z = GetActorLocation().Z;
	FVector newPosition = teleportBlock->teleportDestination->getCenterPoint();
	newPosition.Z = Z;
	SetActorLocation(newPosition);
	currentBlock = teleportBlock->teleportDestination;
	nextBlock = getNextBlock();
}