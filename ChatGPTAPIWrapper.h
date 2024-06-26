// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "UObject/Script.h"
#include "UObject/Object.h"
#include "ChatGPTAPIWrapper.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FonSuccessDelegate, const FString&, GeneratedText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnErrorDelegate, int32, ResponseCode, const FString&, ErrorMessage);

UCLASS(Blueprintable)
class MYPROJECT_API UChatGPTAPIWrapper : public UObject
{
	GENERATED_BODY()

public:
	
	UChatGPTAPIWrapper();

	UFUNCTION(BlueprintCallable, Category="OpenAI")
	void SendRequest(const FString& Prompt, const FonSuccessDelegate& OnSuccess, const FOnErrorDelegate& OnError);

	UFUNCTION(BlueprintCallable, Category="OpenAI")
	void SetOpenAIAPIKey(const FString& ApiKey);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OpenAI")
	int32 MaxTokens = 2048;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OpenAI")
	float Temperature = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="OpenAI")
	float TopP = 0.2f;

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FonSuccessDelegate OnSuccess, FOnErrorDelegate OnError);

	UFUNCTION(BlueprintCallable, Category="OpenAI")
	FString GetOpenAIAPIKey();

	FString OpenAIAPIKey;
};
