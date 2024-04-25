// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "LSystem.h"
#include "Branch.h"
#include "Twig.h"
#include "GraphNode.h"
#include "RandomNumberGenerator.h"
#include "TreeGen.generated.h"

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class PROCEDURALTREEGENERATOR_API ATreeGen : public AActor
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	bool bShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	bool bUseRandom = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	bool bMakeTwigs = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	bool bUseInstancing = false;

	// Max amount in degrees to apply random rotation on all axes when going forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 180, UIMin = 0, UIMax = 180), Category = "Tree")
	int RandomAngleMax = 25;

	// Max amount in degrees to apply random rotation on Roll axis when branching.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	int BranchRollMax = 137;

	// Scale at which the tree will begin with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float StartWidthScale = 1.f;

	// Minimum width allowed for branch segments. Each segments smaller than this will not produce a spline mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1), Category = "Tree")
	float MinWidthScale = 0.1f;

	// Scales width by this amount when going forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float WidthScaleFactor = 0.9f;

	// Scale at which branch tips will terminate at.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float BranchTipScale = 0.05f;

	// Scales width by this amount when creating new branch.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float BranchingWidthScaleFactor = 0.5f;

	// Scales length by this amount when creating new branch.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float BranchingLengthFactor = 0.7f;

	// Average number of steps to be traversed along tree until a twig is created.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	int TwigSpawnPerAvgSteps = 4;

	// Threshold length past which twigs can spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	float TwigStartThreshold = 300.f;

	// Default twig length.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	float TwigLength = 100.f;

	// Minimum scale for twigs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	float MinTwigScale = 0.4f;

	// Twig scale growth per segment from branch tip. Clamped between MinTwigScale and 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	float TwigScalePerSegment = 0.1f;

	// Minimum distance in segments from branch tip for.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	int MinDistanceFromTipForTwig = 2;

	// Length by which each forward step should travel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float Length = 50.f;

	// Angle by which to rotate Yaw on '+' and '-'.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	float Angle = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree")
	UStaticMesh* MeshForTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twig")
	TArray<UStaticMesh*> TwigMeshes;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tree", meta = (AllowPrivateAccess = true))
	TArray<FBranch> Tree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tree", meta = (AllowPrivateAccess = true))
	TArray<USplineMeshComponent*> TreeMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Twig", meta = (AllowPrivateAccess = true))
	TArray<USplineMeshComponent*> SpawnedTwigSplineMeshes;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Twig", meta = (AllowPrivateAccess = true))
	TArray<UInstancedStaticMeshComponent*> TwigInstancers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URandomNumberGenerator* RandomNumberGenerator;
	URandomNumberGenerator* TwigRandomNumberGenerator;

	TSharedPtr<FGraphNode> RootNode;

	int MinBranchSegmentCount = 3;

public:
	UFUNCTION(BlueprintCallable)
	void Init();

private:
	TArray<FVector> TransformsToVectors(TArray<FTransform>& Transforms) const;

	void SetupInstancers();

	void GenerateTree();
	void GenerateSplineMeshes();
	void GenerateTwigs();

	void GenerateTwig(TSharedPtr<FGraphNode> Node);

	void ClearTree();
	void ClearNodes();
	void ClearSplineMeshes();
	void ClearTwigs();

	void RemoveShortBranches();

	UStaticMesh* AssignRandomTwigMesh() const;
	int AssignRandomTwigMeshIndex() const;
};
