// Armand Yilinkou 2024


#include "TreeGen.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// #include "Async/Async.h"
// #include <future>

#include "DrawDebugHelpers.h"
#include "Timer.h"

ATreeGen::ATreeGen()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Turtle = CreateDefaultSubobject<USceneComponent>("Turtle");
	Turtle->SetupAttachment(RootComponent);
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(RootComponent);

	LSystem = CreateDefaultSubobject<ULSystem>("LSystem");
	RandomNumberGenerator = CreateDefaultSubobject<URandomNumberGenerator>("RandomNumberGenerator");
	TwigRandomNumberGenerator = CreateDefaultSubobject<URandomNumberGenerator>("TwigRandomNumberGenerator");
}

void ATreeGen::Init()
{
	Timer FunctionTimer("ATreeGen::Init()");
	
	TwigRandomNumberGenerator->RandomSeed = RandomNumberGenerator->GenerateNumber() % INT32_MAX;
	TwigRandomNumberGenerator->Init();

	GenerateTree();
	
	/*
	if (bUseAsync)
	{
		std::promise<void> SplinesPromise;
		std::future<void> SplinesFuture = SplinesPromise.get_future();
		
		Async(EAsyncExecution::ThreadPool, [this, &SplinesPromise]() {
			this->GenerateSplines();
			SplinesPromise.set_value();
		});

		std::promise<void> TwigsPromise;
		std::future<void> TwigsFuture = TwigsPromise.get_future();

		Async(EAsyncExecution::ThreadPool, [this, &TwigsPromise]() {
			this->GenerateTwigs();
			TwigsPromise.set_value();
		});

		SplinesFuture.wait();
		TwigsFuture.wait();
	}
	else
	{
		GenerateSplines();
		GenerateTwigs();
	}
	*/

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

	Tree.AddDefaulted();
	Tree[0].Points.Add(Turtle->GetRelativeTransform());

	if (bShowDebug)
	{
		DrawDebugSphere(GetWorld(), Turtle->GetComponentLocation(), 12, 6, FColor::Green, false, 10.f);
	}

	int OpenBranchesToIgnore = 0;
	int CurrentBranchIndex = 0;
	float CurrentWidthScale = 1.f;
	float CurrentLengthScale = 1.f;
	float CurrentTotalLength = -NegativeHeightOffset;

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

			if (bUseRandom && RandomAngleMax != 0)
			{
				int RandPitch = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;
				int RandYaw = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;
				int RandRoll = (RandomNumberGenerator->GenerateNumber() % RandomAngleMax) * 2 - RandomAngleMax;

				Turtle->AddLocalRotation(FRotator(RandPitch, RandYaw, RandRoll));
			}

			if (CurrentTotalLength >= TwigStartThreshold && TwigRandomNumberGenerator->GenerateNumber() % TwigSpawnPerAvgSteps == 0)
			{
				int Index = Tree[CurrentBranchIndex].TwigPoints.AddDefaulted();
				Tree[CurrentBranchIndex].TwigPoints[Index].Scale = std::max(0.4f, CurrentWidthScale * 4.f);
				Tree[CurrentBranchIndex].TwigPoints[Index].Location = Turtle->GetRelativeLocation();
				Tree[CurrentBranchIndex].TwigPoints[Index].Tangent = Turtle->GetForwardVector().Cross(Turtle->GetUpVector()) *
					(TwigRandomNumberGenerator->GenerateNumber() % 2 == 0 ? 1 : -1);
			}
			break;
		case 'X':
			break;
		}
	}

	RemoveShortBranches();

	if (bShowDebug)
	{
		for (const FBranch& Branch : Tree)
		{
			for (const FTransform& Transform : Branch.Points)
			{
				DrawDebugSphere(GetWorld(), Transform.GetLocation() + GetActorLocation(), 12, 4, FColor::Green, false, 10.f);
			}

			for (const FTwig& Twig : Branch.TwigPoints)
			{
				DrawDebugSphere(GetWorld(), Twig.Location + GetActorLocation(), 20, 4, FColor::Red, false, 10.f);
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
		//Spline->SetSplinePoints(TransformsToVectors(Branch.Points), ESplineCoordinateSpace::Local);

		for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			if (SplineMesh)
			{
				SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMesh->RegisterComponent();

				if (MeshForSplines)
				{
					SplineMesh->SetStaticMesh(MeshForSplines);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Spline static mesh not set!"));
				}
				
				SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, false);
				//AddInstanceComponent(SplineMesh);

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

	for (const FBranch& Branch : Tree)
	{
		for (const FTwig& Twig : Branch.TwigPoints)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			if (SplineMesh)
			{
				SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMesh->RegisterComponent();

				if (TwigMesh)
				{
					SplineMesh->SetStaticMesh(TwigMesh);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Twig mesh not set!"));
				}

				SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, false);
				//AddInstanceComponent(SplineMesh);

				SplineMesh->SetStartAndEnd(
					Twig.Location,
					Twig.Tangent,
					Twig.Location + (Twig.Tangent * TwigLength * Twig.Scale),
					Twig.Tangent
				);

				SplineMesh->SetStartScale(FVector2D(Twig.Scale));
				SplineMesh->SetEndScale(FVector2D(Twig.Scale));

				TreeMeshes.Add(SplineMesh);
			}
		}
	}
}

void ATreeGen::ClearTree()
{
	Tree.Empty();
}

void ATreeGen::ClearSplines()
{
	for (USplineMeshComponent* SplineMesh : TreeMeshes)
	{
		if (SplineMesh)
		{
			SplineMesh->DestroyComponent();
		}
	}
	TreeMeshes.Empty();

	Spline->ClearSplinePoints();
}

void ATreeGen::ClearTwigs()
{
	for (USplineMeshComponent* TwigComp : TwigMeshes)
	{
		if (TwigComp)
		{
			TwigComp->DestroyComponent();
		}
	}
	TwigMeshes.Empty();
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

TArray<FVector> ATreeGen::TransformsToVectors(TArray<FTransform>& Transforms) const
{
	TArray<FVector> Vectors;
	for (FTransform& Transform : Transforms)
	{
		Vectors.Add(Transform.GetLocation());
	}
	return Vectors;
}
