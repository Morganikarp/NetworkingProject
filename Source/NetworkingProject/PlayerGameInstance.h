// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PlayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKINGPROJECT_API UPlayerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPlayerGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Create_ServerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Create_ServerMaxPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Found_NumOfServers;
	UPROPERTY(EditAnywhere) FString Found_ServerName[10];
	UPROPERTY(EditAnywhere) int32 Found_ServerActivePlayers[10];
	UPROPERTY(EditAnywhere) int32 Found_ServerMaxPlayers[10];
	FOnlineSessionSearchResult Found_ServerList[10];


	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ChosenServerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool IsLANMatch;


protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void FindServers();

	UFUNCTION(BlueprintCallable)
	void JoinServer();
};
