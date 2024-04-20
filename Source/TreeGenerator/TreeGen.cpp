// Armand Yilinkou 2024


#include "TreeGen.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include <stack>

#include "DrawDebugHelpers.h"
#include "Timer.h"

ATreeGen::ATreeGen()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Disable construction script on drag. Stops the entire tree being regenerated when moved.
	UBlueprint* Blueprint = Cast<UBlueprint>(GetClass()->ClassGeneratedBy);
	if (Blueprint)
	{
		Blueprint->bRunConstructionScriptOnDrag = false;
	}

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Turtle = CreateDefaultSubobject<USceneComponent>("Turtle");
	Turtle->SetupAttachment(RootComponent);
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(RootComponent);
	Spline->SetMobility(EComponentMobility::Static);

	LSystem = CreateDefaultSubobject<ULSystem>("LSystem");
	RandomNumberGenerator = CreateDefaultSubobject<URandomNumberGenerator>("RandomNumberGenerator");
	TwigRandomNumberGenerator = CreateDefaultSubobject<URandomNumberGenerator>("TwigRandomNumberGenerator");
}

void ATreeGen::Init()
{
	Timer FunctionTimer("ATreeGen::Init()");
	
	TwigRandomNumberGenerator->RandomSeed = RandomNumberGenerator->GenerateNumber() % INT32_MAX;
	TwigRandomNumberGenerator->Init();

	if (bUseInstancing)
	{
		SetupInstancers();
	}

	GenerateTree();
	GenerateSplines();
	GenerateTwigs();
}

void ATreeGen::GenerateTree()
{
	// Timer FunctionTimer("GenerateTree()");
	
	float NegativeHeightOffset = Length + 10;
	Turtle->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	Turtle->AddLocalRotation(FRotator(90.f, 0.f, 0.f));
	Turtle->AddRelativeLocation(Turtle->GetForwardVector() * -NegativeHeightOffset);

	ClearTree();
	ClearNodes();

	Tree.AddDefaulted();
	Tree[0].Points.Add(Turtle->GetRelativeTransform());

	TSharedPtr<FGraphNode> Node = RootNode;

	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), Turtle->GetComponentLocation(), 12, 6, FColor::Green, false, 10.f);
	}

	int OpenBranchesToIgnore = 0;
	int CurrentBranchIndex = 0;
	float CurrentWidthScale = 1.f;
	float CurrentLengthScale = 1.f;
	float CurrentTotalLength = -NegativeHeightOffset;

	std::stack<TSharedPtr<FGraphNode>> NodeStack;

	// Temp variable used when rotating values
	float BranchTotalLength = 0.f;
	for (size_t i = 0; i < LSystem->CurrentString.Len(); i++)
	{		
		auto Char = LSystem->CurrentString[i];

		switch (Char)
		{
		case '+':
			Turtle->AddLocalRotation(FRotator(0.f, -Angle, 0.f));
			break;
		case '-':
			Turtle->AddLocalRotation(FRotator(0.f, Angle, 0.f));
			break;
		case '[':
			if (CurrentWidthScale < MinWidthScale)
			{
				OpenBranchesToIgnore++;
				break;
			}
			
			if (bUseRandom && BranchRollMax != 0)
			{
				int RandBranchRoll = (RandomNumberGenerator->GenerateNumber() % BranchRollMax) * 2 - BranchRollMax;
				Turtle->AddLocalRotation(FRotator(0.f, 0.f, RandBranchRoll));
			}
			
			{
				int Index = Tree.AddDefaulted();
				Tree[Index].ParentIndex = CurrentBranchIndex;
				Tree[Index].ParentWidthScale = CurrentWidthScale;
				Tree[Index].ParentLengthScale = CurrentLengthScale;
				Tree[Index].TotalLength = CurrentTotalLength;
				Tree[Index].Points.Add(Turtle->GetRelativeTransform());
				CurrentBranchIndex = Index;
			}

			CurrentWidthScale *= BranchingWidthScaleFactor;
			CurrentLengthScale *= BranchingLengthFactor;

			NodeStack.push(Node);
			break;
		case ']':
			if (CurrentWidthScale < MinWidthScale && OpenBranchesToIgnore > 0)
			{
				OpenBranchesToIgnore--;
				break;
			}
			
			Turtle->SetRelativeTransform(Tree[CurrentBranchIndex].Points[0]);
			CurrentWidthScale = Tree[CurrentBranchIndex].ParentWidthScale;
			CurrentLengthScale = Tree[CurrentBranchIndex].ParentLengthScale;
			BranchTotalLength = CurrentTotalLength;
			CurrentTotalLength = Tree[CurrentBranchIndex].TotalLength;
			Tree[CurrentBranchIndex].TotalLength = BranchTotalLength;
			CurrentBranchIndex = Tree[CurrentBranchIndex].ParentIndex;

			Node = NodeStack.top();
			NodeStack.pop();
			break;
		case 'F':
			if (CurrentWidthScale < MinWidthScale)
			{
				break;
			}
			
			Turtle->AddRelativeLocation(Turtle->GetForwardVector() * Length * CurrentLengthScale);
			Tree[CurrentBranchIndex].Points.Add(Turtle->GetRelativeTransform());
			CurrentWidthScale *= WidthScaleFactor;
			CurrentTotalLength += Length * CurrentLengthScale;

			{
				TSharedPtr<FGraphNode> NodePtr = Node->Add();
				int RefCount = NodePtr.GetSharedReferenceCount();

				if (bUseRandom && RandomAngleMax != 0)
				{
					int RandPitch = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;
					int RandYaw = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;
					int RandRoll = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;

					Turtle->AddLocalRotation(FRotator(RandPitch, RandYaw, RandRoll));
				}

				if (CurrentTotalLength >= TwigStartThreshold && TwigRandomNumberGenerator->GenerateNumber() % TwigSpawnPerAvgSteps == 0)
				{
					NodePtr->bHasTwig = true;
					NodePtr->Twig.Location = Turtle->GetRelativeLocation();
					NodePtr->Twig.TwigMesh = AssignRandomTwigMesh();
					NodePtr->Twig.MeshIndex = AssignRandomTwigMeshIndex();

					std::default_random_engine Engine(TwigRandomNumberGenerator->GenerateNumber());
					std::uniform_real_distribution<double> Distribution{ 0, 360 };
					double RotationAngle = Distribution(Engine);

					FVector CrossProduct = Turtle->GetForwardVector().Cross(Turtle->GetUpVector());
					NodePtr->Twig.Tangent = CrossProduct.RotateAngleAxis(RotationAngle, Turtle->GetForwardVector());
					NodePtr->Twig.Rotation = FQuat(UKismetMathLibrary::MakeRotFromZ(NodePtr->Twig.Tangent));
				}
				Node = NodePtr;
			}
			break;
		case 'X':
			break;
		}
	}

	RemoveShortBranches(); // this might cause some problems

	if (bShowDebug)
	{
		for (const FBranch& Branch : Tree)
		{
			for (const FTransform& Transform : Branch.Points)
			{
				DrawDebugSphere(GetWorld(), Transform.GetLocation() + GetActorLocation(), 12, 4, FColor::Green, false, 10.f);
			}
		}
	}
}

void ATreeGen::GenerateSplines()
{
	// Timer FunctionTimer("GenerateSplines");
	
	ClearSplines();
	
	for (FBranch& Branch : Tree)
	{
		float CurrentWidthScale = Branch.ParentWidthScale;

		// Temporary fix for spline folding in on itself on first set of points
		TArray<FTransform>& TempArray = Branch.Points;
		if (Branch.ParentIndex == -1)
		{
			TempArray.RemoveAt(0);
		}
		
		Spline->SetSplinePoints(TransformsToVectors(TempArray), ESplineCoordinateSpace::Local);

		for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
		{
			USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(AddComponentByClass(
				USplineMeshComponent::StaticClass(),
				false,
				FTransform::Identity,
				false)
			);

			if (SplineMesh)
			{
				SplineMesh->SetMobility(EComponentMobility::Static);
				SplineMesh->bDisallowNanite = false;

				if (MeshForTree)
				{
					SplineMesh->SetStaticMesh(MeshForTree);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Tree static mesh not set!"));
					return;
				}
				
				SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, false);

				FVector StartLocation, StartTangent, EndLocation, EndTangent;
				Spline->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
				Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);
				SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

				// close branch on itself by setting scale over the last two spline meshes
				if (i == Spline->GetNumberOfSplinePoints() - 3)
				{
					SplineMesh->SetStartScale(FVector2D(CurrentWidthScale));
					SplineMesh->SetEndScale(FVector2D(CurrentWidthScale / 2.f));
				}
				else if (i == Spline->GetNumberOfSplinePoints() - 2)
				{
					SplineMesh->SetStartScale(FVector2D(CurrentWidthScale / 2.f));
					SplineMesh->SetEndScale(FVector2D(0.f));
				}
				else
				{
					SplineMesh->SetStartScale(FVector2D(CurrentWidthScale));
					CurrentWidthScale *= WidthScaleFactor;
					SplineMesh->SetEndScale(FVector2D(CurrentWidthScale));
				}

				TreeMeshes.Add(SplineMesh);
			}
		}
	}
}

void ATreeGen::GenerateTwigs()
{
	if (!bMakeTwigs)
	{
		return;
	}
	
	// Timer FunctionTimer("GenerateTwigs");
	
	ClearTwigs();
	RootNode->CalculateDistanceFromTip(MinDistanceFromTipForTwig); // also disables twigs on branch tips

	for (const TSharedPtr<FGraphNode> Node : RootNode->ChildNodes)
	{
		GenerateTwig(Node);
	}
}

void ATreeGen::GenerateTwig(TSharedPtr<FGraphNode> Node)
{
	for (TSharedPtr<FGraphNode> ChildNode : Node->ChildNodes)
	{
		GenerateTwig(ChildNode);
	}
	
	if (Node->bHasTwig)
	{
		FTwig& Twig = Node->Twig;
		Twig.Scale = std::min(std::max(MinTwigScale, Node->DistanceFromFurthestTip * TwigScalePerSegment), 1.f);
		float NumA = (TwigRandomNumberGenerator->GenerateNumber() % 6) * 0.1f + 0.5f;
		float NumB = (TwigRandomNumberGenerator->GenerateNumber() % 6) * 0.1f + 0.5f;
		float RandomScale = NumA + NumB;

		if (bUseInstancing)
		{
			if (Twig.MeshIndex == -1)
			{
				return;
			}
			
			UInstancedStaticMeshComponent* Instancer = TwigInstancers[Twig.MeshIndex];
			
			float Scalar = (Twig.Location + Twig.Tangent * TwigLength * Twig.Scale * RandomScale).Length() / Twig.Location.Length();

			FTransform TwigTransform = FTransform::Identity;
			TwigTransform.SetLocation(Twig.Location);
			TwigTransform.SetRotation(Twig.Rotation);
			TwigTransform.SetScale3D(FVector(Scalar));

			int Index = Instancer->AddInstance(TwigTransform);
			return;
		}

		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(AddComponentByClass(
			USplineMeshComponent::StaticClass(),
			false,
			FTransform::Identity,
			false)
		);

		if (SplineMesh)
		{
			SplineMesh->SetMobility(EComponentMobility::Static);
			SplineMesh->bDisallowNanite = false;

			if (Twig.TwigMesh)
			{
				SplineMesh->SetStaticMesh(Twig.TwigMesh);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Twig mesh not set!"));
				return;
			}

			SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, false);
			SplineMesh->SetStartAndEnd(
				Twig.Location,
				Twig.Tangent,
				Twig.Location + (Twig.Tangent * TwigLength * Twig.Scale * RandomScale),
				Twig.Tangent
			);

			SplineMesh->SetStartScale(FVector2D(Twig.Scale));
			SplineMesh->SetEndScale(FVector2D(Twig.Scale));

			SpawnedTwigSplineMeshes.Add(SplineMesh);
		}
		
	}
}

void ATreeGen::ClearTree()
{
	Tree.Empty();
}

void ATreeGen::ClearNodes()
{
	RootNode = MakeShared<FGraphNode>();
}

void ATreeGen::ClearSplines()
{
	if (!TreeMeshes.IsEmpty())
	{
		for (USplineMeshComponent* SplineMesh : TreeMeshes)
		{
			if (SplineMesh)
			{
				SplineMesh->DestroyComponent();
			}
		}
		TreeMeshes.Empty();
	}

	Spline->ClearSplinePoints();
}

void ATreeGen::ClearTwigs()
{
	for (USplineMeshComponent* TwigComp : SpawnedTwigSplineMeshes)
	{
		if (TwigComp)
		{
			TwigComp->DestroyComponent();
		}
	}
	SpawnedTwigSplineMeshes.Empty();
}

void ATreeGen::RemoveShortBranches()
{
	for (int i = Tree.Num() - 1; i >= 0 ; i--)
	{
		if (Tree[i].Points.Num() < MinBranchSegmentCount)
		{
			Tree.RemoveAt(i);
		}
	}
}

UStaticMesh* ATreeGen::AssignRandomTwigMesh() const
{
	if (TwigMeshes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No twig meshes assigned!"));
		return nullptr;
	}
	
	std::default_random_engine Engine(TwigRandomNumberGenerator->GenerateNumber());
	std::uniform_int_distribution<int> Distribution{ 0, TwigMeshes.Num() - 1};

	int Index = Distribution(Engine);
	return TwigMeshes[Index];
}

int ATreeGen::AssignRandomTwigMeshIndex() const
{
	if (TwigInstancers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No instance twig meshes assigned!"));
		return -1;
	}

	std::default_random_engine Engine(TwigRandomNumberGenerator->GenerateNumber());
	std::uniform_int_distribution<int> Distribution{ 0, TwigInstancers.Num() - 1 };

	return Distribution(Engine);
}

TArray<FVector> ATreeGen::TransformsToVectors(TArray<FTransform>& Transforms) const
{
	TArray<FVector> Vectors;
	for (FTransform& Transform : Transforms)
	{
		Vectors.Add(Transform.GetLocation());
	}
	return Vectors;
}

void ATreeGen::SetupInstancers()
{
	TwigInstancers.Empty();

	for (UStaticMesh* Mesh : TwigMeshes)
	{
		if (!Mesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Twig mesh not set!"));
			return;
		}
		
		UInstancedStaticMeshComponent* Instancer = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(
			UInstancedStaticMeshComponent::StaticClass(),
			false,
			FTransform::Identity,
			false)
		);

		Instancer->SetStaticMesh(Mesh);
		Instancer->SetMobility(EComponentMobility::Static);
		Instancer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Instancer->bDisallowNanite = false;

		TwigInstancers.Add(Instancer);
	}
}
