// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Actor.h"
#include <MixerInteractivityModule.h>
#include <MixerInteractivityTypes.h>
#include "Engine/DynamicBlueprintBinding.h"
#include "Delegates/Delegate.h"
#include "MixerGameClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FParticipantJoinDelegate, int32, participantID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FParticipantLeaveDelegate, int32, participantID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMixerLoginStateChangedDelegate, bool, isLoggedOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FParticipantClickDelegate, int32, participantID, float, x, float, y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FParticipantEventDelegate, int32, participantID, FString, participantName, FString, code, float, x, float, y);

UENUM(BlueprintType)
enum class EStatCategory : uint8
{
	SCORE			UMETA(DisplayName = "Score"),
	HITS			UMETA(DisplayName = "Hits"),
	HEADSHOTS		UMETA(DisplayName = "Headshots"),
	DAMAGE_DEALT	UMETA(DisplayName = "Damage Dealt"),
	KILLS			UMETA(DisplayName = "Kills"),

	MAX_CATEGORIES	UMETA(Hidden),
};

USTRUCT()
struct FSubSessionWinner
{
	GENERATED_BODY()

	UPROPERTY()
		int32 ParticipantID;

};

UCLASS()
class MIXBLANK_API AMixerGameClient : public AActor
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		int32 MYGUID;

	// Sets default values for this actor's properties
	AMixerGameClient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnLoginStateChanged(EMixerLoginState state);
	void OnCustomControlInput(FName ControlName, FName EventType, TSharedPtr<const FMixerRemoteUser> Participant, TSharedRef<FJsonObject> EventPayload);
	void OnParticipantStateChanged(TSharedPtr<const FMixerRemoteUser> Participant, EMixerInteractivityParticipantState participantState);
	//void OnButtonEvent(FName ButtonName, TSharedPtr<const FMixerRemoteUser> Participant, const FMixerButtonEventDetails& Details);

	UFUNCTION(BlueprintCallable, Category = "MixBlankGameClient")
		static void SendParticipantUpdate(int32 participantID, int32 numClicks);

	UFUNCTION(BlueprintCallable, Category = "MixBlankGameClient")
		static void UpdateControlState(int32 numClicks);

	static TSharedPtr<FJsonObject> PackHeader(int32 playerID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MixBlankGameClient")
		static bool GetIsPlayerLoggedIn();

	//	generic more flexible participant event
	UFUNCTION(BlueprintCallable, Category = "MixBlankGameClient")
		static void SendBroadcastEvent(const FString &code, float x, float y, const FString &variant);

	//	generic more flexible participant event
	UFUNCTION(BlueprintCallable, Category = "MixBlankGameClient")
		static void SendParticipantEvent(int32 participantID, const FString &code, float x, float y, const FString &variant);


	//	new approach : delegate
	UPROPERTY(BlueprintAssignable)
		FParticipantJoinDelegate OnParticipantJoin;
	UPROPERTY(BlueprintAssignable)
		FParticipantLeaveDelegate OnParticipantLeave;
	UPROPERTY(BlueprintAssignable)
		FParticipantEventDelegate OnParticipantEvent;
	UPROPERTY(BlueprintAssignable)
		FMixerLoginStateChangedDelegate OnMixerLogginStateChanged;

private:

};
DECLARE_LOG_CATEGORY_EXTERN(LogMixerGC, Log, All);
