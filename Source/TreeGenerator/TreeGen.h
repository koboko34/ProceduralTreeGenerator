// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "LSystem.h"
#include "Branch.h"
#include "RandomNumberGenerator.h"
#include "TreeGen.generated.h"

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class TREEGENERATOR_API ATreeGen : public AActor
{
	GENERATED_BODY()

	ATreeGen();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* Turtle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULSystem* LSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseRandom = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = 25.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentAngle = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* MeshForSplines;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FBranch> Tree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URandomNumberGenerator* RandomNumberGenerator;

public:
	UFUNCTION(BlueprintCallable)
	void GenerateTree();
	UFUNCTION(BlueprintCallable)
	void GenerateSplines();

private:
	TArray<FVector> TransformsToVectors(TArray<FTransform>& Transforms) const;
};
