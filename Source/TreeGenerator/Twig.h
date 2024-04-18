// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Twig.generated.h"

USTRUCT(BlueprintType)
struct FTwig
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Tangent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* TwigMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 1.f;
};