// Copyright 2019 Rainy Labs, LLC.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WHT_FunctionLibrary.generated.h"

//=======================================================
//Log Macro
//=======================================================

//log a string
#define log(text) UE_LOG(LogTemp, Log, TEXT("%s"), text);

//Logs warning
#define logWarning(text) UE_LOG(LogTemp, Warning, TEXT("%s"), text);

//Logs error
#define logError(text) UE_LOG(LogTemp, Error, TEXT("%s"), text);

/**
 * 
 */
UCLASS()
class WRAPPERHELPERTEMPATE_API UWHT_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Test function to make sure our library loaded
	UFUNCTION(BlueprintCallable, Category = "Testing")
		static float TestSampleFunction(float Param);
	
};
