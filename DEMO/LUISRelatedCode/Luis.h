// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PythonComponent.h"
#include "Luis.generated.h"



class FLuisTask : public FNonAbandonableTask
{
	friend class FAsyncTask<FLuisTask>;

public:
	FLuisTask(TArray<TFunction<void()>> funcs);
	~FLuisTask();

	//required by ue4, is required
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FLuisTask, STATGROUP_ThreadPoolAsyncTasks)
	}

	void DoWork();
private:
	TArray<TFunction<void()>> func;
};

UENUM(BlueprintType)
enum class ELuisLangugae : uint8 {
	ar_AR		UMETA(DisplayName = "Arabic"),
	zh_CH		UMETA(DisplayName = "Chinese"),
	nl_NL		UMETA(DisplayName = "Dutch"),
	en_US		UMETA(DisplayName = "English_UnitedStates"),
	fr_CA		UMETA(DisplayName = "French_Canada"),
	fr_FR		UMETA(DisplayName = "French_France"),
	de_DE		UMETA(DisplayName = "German"),
	gu_IN		UMETA(DisplayName = "Gujarati"),
	hi_IN		UMETA(DisplayName = "Hindi"),
	it_IT		UMETA(DisplayName = "Italian"),
	ja_JP		UMETA(DisplayName = "Japanese"),
	ko_KR		UMETA(DisplayName = "Korean"),
	mr_IN		UMETA(DisplayName = "Marathi"),
	pt_BR		UMETA(DisplayName = "Portuguese_Brazil"),
	es_MX		UMETA(DisplayName = "Spanish_Mexico"),
	es_ES		UMETA(DisplayName = "Spanish_Spain"),
	ta_IN		UMETA(DisplayName = "Tamil"),
	te_IN		UMETA(DisplayName = "Telugu"),
	tr_TR		UMETA(DisplayName = "Turkish")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLuisStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLuised);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLuisFinished, const TArray<FString>&, Entities, FString, Topintent, float, Score, bool, Success);

UCLASS()
class AZURESPEECHSDK_API ALuis : public AActor
{
	GENERATED_BODY()
private:
	FAsyncTask<FLuisTask>* luisTask;
	UPythonComponent* pythonComponent;

public:
	// Sets default values for this actor's properties
	ALuis();
	~ALuis();

	TArray<TFunction<void()>> Work;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//luis need infomation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis Set")
		FString AuthoringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis Set")
		FString PredictionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis Set")
		FString AuthoringEndpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis Set")
		FString PredictionEndpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis Set")
		FString AppId;
	

	UFUNCTION(BlueprintCallable, Category = "Luis")
		FString GetStringEnumLanguage(ELuisLangugae language);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Luis")
		ELuisLangugae TargetLanguage = ELuisLangugae::en_US;

	UFUNCTION(BlueprintCallable, Category = "Luis")
		void RunLuisCognition();

	UPROPERTY(VisibleAnywhere, Category = "Luis Intent")
		FString IntentName;

	//luis result score	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		FString Utterance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		float IntentScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		FString TopIntentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		TArray<FString>  EntitieList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		FString FileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Luis")
		FString appName;

	UFUNCTION()
		void BroadcastStarted();

	UFUNCTION()
		void BroadcastRecognizing();

	UFUNCTION()
		void BroadcastFinished(const TArray<FString>& entities, FString topIntent, float score, bool success);
	
	//luis app
	UFUNCTION(BlueprintCallable)
		void LuisBeginPlay();

	UFUNCTION(BlueprintCallable)
		void LuisAddIntent(FString _intentName);

	UFUNCTION(BlueprintCallable)
		void LuisAddEntities(FString _fileName);

	UFUNCTION(BlueprintCallable)
		void LuisCreateApp(FString _appName);

	UFUNCTION(BlueprintCallable)
		void LuisSetting(FString _AuthoringKey, FString _PredictionKey, FString _AuthoringEndpoint, FString _PredictionEndpoint, FString _AppId, ELuisLangugae _langugae);

	//luis intent score
	UFUNCTION(BlueprintCallable)
		void LuisUtteranceScore(FString _utterance);


	// Delegate
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Luis")
		FLuisStart Started;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Luis")
		FLuised Recognizing;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Luis")
		FLuisFinished Finished;
	
};
