// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameInstance.h"

#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include <Online/OnlineSessionNames.h>

UPlayerGameInstance::UPlayerGameInstance()
{
}

void UPlayerGameInstance::Init() //Runs immediatly
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{

		SessionInterface = SubSystem->GetSessionInterface();

		if (SessionInterface.IsValid()) {

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("bvruh"));

			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlayerGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlayerGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPlayerGameInstance::OnJoinSessionComplete);
		}
	}
}

void UPlayerGameInstance::CreateServer() //Runs once after Create Server button is clicked
{

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = Create_ServerMaxPlayers;


	SessionInterface->CreateSession(0, FName(Create_ServerName), SessionSettings);
}

void UPlayerGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded) //Runs once after SessionInterface->CreateSession
{
	if (Succeeded) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnCreateSessionComplete, Succeeded"));

		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnCreateSessionComplete, Failed"));
	}

}

void UPlayerGameInstance::FindServers() //Runs once after Refresh button is clicked
{

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UPlayerGameInstance::OnFindSessionsComplete(bool Succeeded) //Runs once after SessionInterface->FindSessions
{
	if (Succeeded)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnFindSessionsComplete, Succeeded"));

		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		Found_NumOfServers = SearchResults.Num();

		if (Found_NumOfServers > 0)
		{
			for (size_t i = 0; i < Found_NumOfServers; i++)
			{
				Found_ServerList[i] = SearchResults[i];
				Found_ServerName[i] = SearchResults[i].Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString();
				Found_ServerActivePlayers[i] = SearchResults[i].Session.SessionSettings.NumPublicConnections;
				Found_ServerMaxPlayers[i] = SearchResults[i].Session.NumOpenPublicConnections;
			}
		}
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnFindSessionsComplete, Failed"));
	}
}

void UPlayerGameInstance::JoinServer() //Runs once after Join Server button is clicked
{
	char* ServerName = TCHAR_TO_ANSI(*Found_ServerName[ChosenServerIndex]);
	SessionInterface->JoinSession(0, ServerName, Found_ServerList[ChosenServerIndex]);
}

void UPlayerGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) //Runs once after SessionInterface->JoinSession 
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "") {
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("I'm so fucking cool"));
		}
	}
}
