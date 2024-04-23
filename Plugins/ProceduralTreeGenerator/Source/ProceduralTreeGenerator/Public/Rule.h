// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Rule.generated.h"

USTRUCT(BlueprintType)
struct FRule
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Element = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProductionRule = "";
};