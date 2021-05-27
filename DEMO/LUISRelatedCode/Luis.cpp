// Fill out your copyright notice in the Description page of Project Settings.


#include "Luis.h"

FLuisTask::FLuisTask(TArray<TFunction<void()>> funcs)
{
	this->func = funcs;
}

FLuisTask::~FLuisTask()
{

}

void FLuisTask::DoWork()
{
	for (auto& item : func) {
		item();
	}
}

// Sets default values
ALuis::ALuis()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pythonComponent = CreateDefaultSubobject<UPythonComponent>(TEXT("python"));
	pythonComponent->PythonModule = "LuisCognitive";
	pythonComponent->PythonClass = "Luis";

}


//¼Ò¸êÀÚ luis delete
ALuis::~ALuis()
{
	if (luisTask) {
		luisTask->EnsureCompletion();
		luisTask->Cancel();
		delete luisTask;		
	}
}

// Called when the game starts or when spawned
void ALuis::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALuis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (luisTask && luisTask->IsDone()) {
		luisTask->EnsureCompletion();
		delete luisTask;
		luisTask = nullptr;
	}
}


//i need using Thread pool one by one
void ALuis::RunLuisCognition()
{
	if (luisTask) {
		return;
	}
	else {
		luisTask = new FAsyncTask<FLuisTask>(Work);
		luisTask->StartBackgroundTask();
	}
}


//Using pyhon script
void ALuis::LuisBeginPlay()
{
	TFunction<void()> work = [this] {
		pythonComponent->CallPythonComponentMethodString("begin_play", "");
		UE_LOG(LogTemp, Warning, TEXT("begin_play"));
	};
	Work.Add(work);
}

void ALuis::LuisAddIntent(FString _intentName)
{
	this->IntentName = _intentName;
	TFunction<void()> work = [this] {		
		pythonComponent->CallPythonComponentMethodString("add_intent", "");
		UE_LOG(LogTemp, Warning, TEXT("add_intent Intent name"));
	};
	Work.Add(work);
}


void ALuis::LuisAddEntities(FString _fileName)
{
	this->FileName = _fileName;	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FileName);
	TFunction<void()> work = [this] {
		pythonComponent->CallPythonComponentMethodString("add_entities", "");		
	};
	Work.Add(work);
}

void ALuis::LuisCreateApp(FString _appName)
{
	this->appName = _appName;
	TFunction<void()> work = [this] {
		//pythonComponent->SetPythonAttrString("appName", appName);
		pythonComponent->CallPythonComponentMethodString("create_app", "");
	};
	Work.Add(work);
}


void ALuis::LuisSetting(FString _AuthoringKey, FString _PredictionKey, FString _AuthoringEndpoint, FString _PredictionEndpoint, FString _AppId , ELuisLangugae _langugae)
{
	
	this->AuthoringKey = _AuthoringKey;
	this->PredictionKey = _PredictionKey;
	this->AuthoringEndpoint = _AuthoringEndpoint;
	this->PredictionEndpoint = _PredictionEndpoint;
	this->AppId = _AppId;

	this->TargetLanguage = _langugae;
	
	TFunction<void()> work = [this] {
		pythonComponent->CallPythonComponentMethodString("set_luis", "");
		UE_LOG(LogTemp, Warning, TEXT("set_luis"));
	};
	Work.Add(work);
}


void ALuis::LuisUtteranceScore(FString _utterance)
{
	this->Utterance = _utterance;

	TFunction<void()> work = [this] {		
		pythonComponent->CallPythonComponentMethodString("from_luis", "");
		UE_LOG(LogTemp, Warning, TEXT("from_luis"));
	};
	Work.Add(work);	
	UE_LOG(LogTemp, Warning, TEXT("%s"),*Utterance);
}


void ALuis::BroadcastStarted()
{
	Started.Broadcast();
}

void ALuis::BroadcastRecognizing()
{
	Recognizing.Broadcast();
}

void ALuis::BroadcastFinished(const TArray<FString>& entities, FString topIntent, float score, bool success)
{
	AsyncTask(ENamedThreads::GameThread, [=]()
	{
		Finished.Broadcast(entities, topIntent, score, success);
	});
}

FString ALuis::GetStringEnumLanguage(ELuisLangugae language)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELuisLangugae"), true);
	if (!enumPtr) {
		return FString("Invalid");
	}

	FString lan = enumPtr->GetNameStringByValue((int32)language);
	lan.ReplaceCharInline('_', '-');
	return lan;
}
