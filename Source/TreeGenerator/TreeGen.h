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

	// Max amount in degrees to apply random rotation on all axes when going forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 180, UIMin = 0, UIMax = 180))
	int RandomAngleMax = 25;

	// Max amount in degrees to apply random rotation on Roll axis when branching.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BranchRollMax = 137;

	// Minimum width allowed for branch segments. Each segments smaller than this will not produce a spline mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float MinWidthScale = 0.1f;

	// Scales width by this amount when going forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WidthScaleFactor = 0.9f;

	// Scales width by this amount when creating new branch.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BranchingWidthScaleFactor = 0.5f;

	// Scales length by this amount when creating new branch.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BranchingLengthFactor = 0.7f;

	// Length by which each forward step should travel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length = 50.f;

	// Angle by which to rotate Yaw on '+' and '-'.
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
