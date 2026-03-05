// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableWall.generated.h"

UCLASS()
class UE57_RPC_CPP_API AMovableWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableWall();
	void Launch();
	void Close();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* Wall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TargetLocationComponent; 

	bool bIsPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	FVector OriginalLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovableWall | Settings")
	FVector MoveTargetLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
