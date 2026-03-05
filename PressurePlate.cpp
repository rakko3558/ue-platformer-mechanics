// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "MovableWall.h"
// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PressurePlate"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshAsset.Succeeded())
	{
		Plate->SetStaticMesh(MeshAsset.Object);
		UE_LOG(LogTemp, Warning, TEXT("CUBE MESH LOAD SUCCESS"));
	}

	BtnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	BtnCollision->SetupAttachment(Plate);
	BtnCollision->SetCollisionProfileName(TEXT("Trigger"));
	BtnCollision->SetRelativeScale3D(FVector(1.6,1.6,5.0));
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	BtnCollision->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	BtnCollision->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != this)
	{

		AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			if (TargetWall)
			{
				TargetWall->Launch();

				UE_LOG(LogTemp, Warning, TEXT("Move"));
			}
		}
	}
}

void APressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{

		AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			if (TargetWall)
			{
				TargetWall->Close();

				UE_LOG(LogTemp, Warning, TEXT("Back"));
			}
		}
	}
}

// Called every frame