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
	FQuat Rotation = FQuat::Identity;

	// used by original method, instancing will use a mesh index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* TwigMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeshIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 1.f;
};