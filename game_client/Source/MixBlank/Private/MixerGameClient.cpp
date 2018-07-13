// TODO copyright

#include "MixerGameClient.h"
#include "MixerInteractivityBlueprintLibrary.h"
#include "MixerInteractivitySettings.h"
#include "BufferArchive.h"
#include <json.h>

DEFINE_LOG_CATEGORY(LogMixerGC);

// Sets default values
AMixerGameClient::AMixerGameClient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMixerGameClient::BeginPlay()
{
	Super::BeginPlay();

	// Set up handlers for mixer events
	FDelegateHandle LoginStateDelegateHandle = IMixerInteractivityModule::Get().OnLoginStateChanged().AddUObject(this, &AMixerGameClient::OnLoginStateChanged);
	FDelegateHandle CustomControlInputDelegateHandle = IMixerInteractivityModule::Get().OnCustomControlInput().AddUObject(this, &AMixerGameClient::OnCustomControlInput);
	////FDelegateHandle ButtonEventDelegateHandle = IMixerInteractivityModule::Get().OnButtonEvent().AddUObject(this, &AMixerGameClient::OnButtonEvent);
	FDelegateHandle ParticipantSateChangedDelegateHandle = IMixerInteractivityModule::Get().OnParticipantStateChanged().AddUObject(this, &AMixerGameClient::OnParticipantStateChanged);

	
	UE_LOG(LogMixerGC, Log, TEXT("MGC v0.0007 Started"));
	
	const UMixerInteractivitySettings* Settings = GetDefault<UMixerInteractivitySettings>();
	UE_LOG(LogMixerGC, Log, TEXT("Game Version ID: %s"), *FString::FromInt(Settings->GameVersionId));
}



// Called every frame
void AMixerGameClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


bool AMixerGameClient::GetIsPlayerLoggedIn()
{
	return IMixerInteractivityModule::Get().GetLoginState() == EMixerLoginState::Logged_In;
}

void AMixerGameClient::OnLoginStateChanged(EMixerLoginState newState)
{
	UE_LOG(LogMixerGC, Log, TEXT("MGC Login State Changed"));
	if (newState == EMixerLoginState::Logged_In)
	{
		IMixerInteractivityModule::Get().StartInteractivity();
		UE_LOG(LogMixerGC, Log, TEXT("MGC Logged In"));

		OnMixerLogginStateChanged.Broadcast(true);
	}
	else if (newState == EMixerLoginState::Not_Logged_In)
	{
		OnMixerLogginStateChanged.Broadcast(false);
	}
}

void AMixerGameClient::OnParticipantStateChanged(TSharedPtr<const FMixerRemoteUser> Participant, EMixerInteractivityParticipantState participantState)
{

	//NOTE:This event is never broadcast as of version v0.9.1-beta of the MixerInteractivity Plugin. 
	//You can however modify MixerInteractivityModule_InteractiveCpp2.cpp to broadcast the InteractiveModule.OnParticipantStateChanged event in the OnSessionParticipantsChanged function where relevant.
	if (Participant.IsValid())
	{
		if (participantState == EMixerInteractivityParticipantState::Joined)
		{
			OnParticipantJoin.Broadcast(Participant->Id);
			UE_LOG(LogMixerGC, Log, TEXT("Participant %s (%d) joined"), *Participant->Name, Participant->Id);
		}
		else
		{
			OnParticipantLeave.Broadcast(Participant->Id);
			UE_LOG(LogMixerGC, Log, TEXT("Participant %s (%d) left"), *Participant->Name, Participant->Id);
		}
	}
}

void AMixerGameClient::OnCustomControlInput(FName ControlName, FName EventType, TSharedPtr<const FMixerRemoteUser> Participant, TSharedRef<FJsonObject> EventPayload)
{
	const TSharedPtr<FJsonObject>* messageObject;
	if (EventPayload->TryGetObjectField(TEXT("message"), /*out*/ messageObject))
	{
		FString codeString;
		if (!(*messageObject)->TryGetStringField("code", codeString))
		{
			UE_LOG(LogMixerGC, Error, TEXT("custom control input has no code string"));
			return;
		}
		UE_LOG(LogMixerGC, Log, TEXT("code : %s, from %s (%d)"), *codeString, *Participant->Name, Participant->Id);
		
		//	many events have a location, so look for that, too.
		double x = 0;
		double y = 0;
		if (!(*messageObject)->TryGetNumberField(TEXT("x"), x)) {
			// there was no x for some reason
		}

		if (!(*messageObject)->TryGetNumberField(TEXT("y"), y)) {
			// there was no y for some reason
		}

		OnParticipantEvent.Broadcast(Participant->Id, Participant->Name, codeString, x, y);

	}
}

//void AMixerGameClient::OnButtonEvent(FName ButtonName, TSharedPtr<const FMixerRemoteUser> Participant, const FMixerButtonEventDetails& Details) {}


TSharedPtr<FJsonObject> AMixerGameClient::PackHeader(int32 participantID)
{
	//	start with basics...
	//	see https://answers.unrealengine.com/questions/80902/how-i-can-parse-json-data-with-a-standard-rountine.html

	TSharedPtr<const FMixerRemoteUser> User = IMixerInteractivityModule::Get().GetParticipant(participantID);
	if (User.IsValid())
	{
		FString guid = User->SessionGuid.ToString(EGuidFormats::DigitsWithHyphens).ToLower();

		FString JsonRaw = "{ \"scope\":[\"participant:" + guid + "\"]}";
		TSharedPtr<FJsonObject> params;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

		FJsonSerializer::Deserialize(JsonReader, params);
		return params;
	}
	
	return NULL;
}

//	generic event for Jeremy
void  AMixerGameClient::SendBroadcastEvent(const FString &code, float x, float y, const FString &variant)
{
	UE_LOG(LogMixerGC, Log, TEXT("SendBroadcastEvent, code: %s, x: %f, y: %f, variant: %s"), *code, x, y);


	FString JsonRaw = "{ \"scope\":[\"group:default\"]}";
	TSharedPtr<FJsonObject> params;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	FJsonSerializer::Deserialize(JsonReader, params);
	TSharedPtr< FJsonObject > bodyJson = MakeShareable(new FJsonObject);
	//	add props...
	bodyJson->SetStringField("code", *code);
	bodyJson->SetStringField("variant", *variant);
	bodyJson->SetNumberField("x", x);
	bodyJson->SetNumberField("y", y);

	params->SetObjectField("data", bodyJson);

	IMixerInteractivityModule::Get().CallRemoteMethod(FString(TEXT("broadcastEvent")), params.ToSharedRef());
}

//	generic event for Jeremy
void AMixerGameClient::SendParticipantEvent(int32 participantID, const FString &code, float x, float y, const FString &variant)
{
	UE_LOG(LogMixerGC, Log, TEXT("SendParticipantEvent, pid: %d, code: %s, x: %f, y: %f"), participantID, *code, x, y);
	
	TSharedPtr<FJsonObject> params = AMixerGameClient::PackHeader(participantID);
	if (!params.IsValid())
		return;

	TSharedPtr< FJsonObject > bodyJson = MakeShareable(new FJsonObject);
	//	add props...
	bodyJson->SetStringField("code", *code);
	bodyJson->SetStringField("variant", *variant);
	bodyJson->SetNumberField("participantID", participantID);
	bodyJson->SetNumberField("x", x);
	bodyJson->SetNumberField("y", y);

	params->SetObjectField("data", bodyJson);

	IMixerInteractivityModule::Get().CallRemoteMethod(FString(TEXT("broadcastEvent")), params.ToSharedRef());

	/*
	see https://answers.unrealengine.com/questions/341767/how-to-add-an-array-of-json-objects-to-a-json-obje.html
	TSharedPtr<FJsonObject> paramsJson;
	paramsJson->SetStringField("scope", array thing);

	IMixerInteractivityModule::Get().CallRemoteMethod(FString(TEXT("broadcastEvent")), const TSharedRef<FJsonObject> MethodParams)
	*/
	/*
	'broadcastEvent',
	{
	scope:["everyone"],
	data:message,
	});
	*/
}


void AMixerGameClient::UpdateControlState(int32 numClicks)
{
	UE_LOG(LogMixerGC, Log, TEXT("Updating anchor control state"));

	const TSharedPtr<FJsonObject> baseObj(new FJsonObject());
	{
		baseObj->SetNumberField("numGlobalClicks", numClicks);
	}

	//assume control ID as anchor
	IMixerInteractivityModule::Get().UpdateRemoteControl(TEXT("default"), TEXT("anchor"), baseObj.ToSharedRef());
}

void AMixerGameClient::SendParticipantUpdate(int32 participantID, int32 numClicks)
{
	UE_LOG(LogMixerGC, Log, TEXT("Updating participant %d"), participantID);

	TSharedPtr<const FMixerRemoteUser> remoteUser = IMixerInteractivityModule::Get().GetParticipant(participantID);
	if (!remoteUser.IsValid())
	{
		return;
	}

	TSharedPtr<FJsonObject> dataObj = AMixerGameClient::PackHeader(participantID);
	{
		if (!dataObj.IsValid())
		{
			return;
		}

		TArray<TSharedPtr<FJsonValue>> participantsArray;
		{
			TSharedPtr<FJsonObject> participantData = MakeShareable(new FJsonObject);
			{
				participantData->SetNumberField(TEXT("clicks"), numClicks);

				FString guid = remoteUser->SessionGuid.ToString(EGuidFormats::DigitsWithHyphens).ToLower();
				participantData->SetStringField(TEXT("sessionID"), guid);
			}
			participantsArray.Add(MakeShareable(new FJsonValueObject(participantData)));
		}
		dataObj->SetArrayField(TEXT("participants"), participantsArray);

		dataObj->SetNumberField(TEXT("priority"), 0);
	}
	IMixerInteractivityModule::Get().CallRemoteMethod(FString(TEXT("updateParticipants")), dataObj.ToSharedRef());
}
