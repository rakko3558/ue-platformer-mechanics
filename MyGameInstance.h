// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

UCLASS()
class UE57_RPC_CPP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UMyGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void CreateGameSession();

protected:

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void Init() override;
    IOnlineSessionPtr SessionInterface;


    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
};
