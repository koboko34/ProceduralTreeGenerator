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
	bool bShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseRandom = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 180, UIMin = 0, UIMax = 180))
	int RandomAngleMax = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BranchRollMax = 137;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinWidthScale = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WidthScaleFactor = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BranchingWidthScaleFactor = 0.5f;
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
