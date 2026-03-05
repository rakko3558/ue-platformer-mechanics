// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
UMyGameInstance::UMyGameInstance()
{
	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnCreateSessionComplete);
}

void UMyGameInstance::Init() {
    Super::Init();
    CreateGameSession(); 
}

void UMyGameInstance::CreateGameSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("cant find Subsystem"));
        return;
    }

    SessionInterface = Subsystem->GetSessionInterface();
    if (!SessionInterface.IsValid())
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SessionInterface valid"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
    if (!LocalPlayer)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("error cant find LocalPlayer"));
        return;
    }

    SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = false;
    SessionSettings.NumPublicConnections = 4;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;

    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);

    if (GEngine)
    {
        FString SubName = Subsystem->GetSubsystemName().ToString();
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("system link: %s"), *SubName));
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("suplying Steam..."));
    }
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("succeed..."));

      
        UGameplayStatics::OpenLevel(GetWorld(), FName("Lvl_ThirdPerson"), true, "listen");
    }
}