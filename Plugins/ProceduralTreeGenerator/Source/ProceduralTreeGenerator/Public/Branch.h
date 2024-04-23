// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Twig.h"
#include "Branch.generated.h"

USTRUCT(BlueprintType)
struct FBranch
{
public:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FTransform> Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ParentWidthScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ParentLengthScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalLength = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ParentIndex = -1;
};