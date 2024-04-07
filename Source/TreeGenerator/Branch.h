// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Branch.generated.h"

USTRUCT(BlueprintType)
struct FBranch
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ParentIndex = -1;
};