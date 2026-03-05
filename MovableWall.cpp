// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableWall.h"

// Sets default values
AMovableWall::AMovableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SUCCESS: Wall.Succeeded"));
		Wall->SetStaticMesh(MeshAsset.Object);
	}


	TargetLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

// Called when the game starts or when spawned
void AMovableWall::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = Wall->GetRelativeLocation();
	MoveTargetLocation = TargetLocationComponent->GetRelativeLocation();
}

// Called every frame
void AMovableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLoc = Wall->GetRelativeLocation();
	FVector TargetLoc = bIsPressed ? MoveTargetLocation : OriginalLocation;

	FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, 5.0f);
	Wall->SetRelativeLocation(NewLoc);

}
void AMovableWall::Launch()
{
	bIsPressed = true;

}
void AMovableWall::Close() 
{
	bIsPressed = false;
}

