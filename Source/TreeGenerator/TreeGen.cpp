// Armand Yilinkou 2024


#include "TreeGen.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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

	// UKismetSystemLibrary::FlushPersistentDebugLines(this);
}

void ATreeGen::GenerateTree()
{
	Tree.Empty();
	Turtle->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	
	Turtle->AddLocalRotation(FRotator(90.f, 0.f, 0.f));

	Tree.AddDefaulted();
	Tree[0].Points.Add(Turtle->GetRelativeTransform());

	DrawDebugSphere(GetWorld(), Turtle->GetComponentLocation(), 12, 6, FColor::Green, false, 10.f);

	int CurrentBranchIndex = 0;
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
			Tree.AddDefaulted();
			Tree[Tree.Num() - 1].ParentIndex = CurrentBranchIndex;
			Tree[Tree.Num() - 1].Points.Add(Turtle->GetRelativeTransform());
			CurrentBranchIndex = Tree.Num() - 1;
			break;
		case ']':
			Turtle->SetRelativeTransform(Tree[CurrentBranchIndex].Points[0]);
			CurrentBranchIndex = Tree[CurrentBranchIndex].ParentIndex;
			break;
		case 'F':
			Turtle->AddRelativeLocation(Turtle->GetForwardVector() * Length);
			Tree[CurrentBranchIndex].Points.Add(Turtle->GetRelativeTransform());

			DrawDebugSphere(GetWorld(), Turtle->GetComponentLocation(), 12, 4, FColor::Green, false, 10.f);
			break;
		case 'X':
			break;
		}
	}
}

void ATreeGen::GenerateSplines()
{
	for (USplineMeshComponent* SplineMesh : SplineMeshes)
	{
		if (SplineMesh)
		{
			SplineMesh->DestroyComponent();
		}
	}
	SplineMeshes.Empty();
	Spline->ClearSplinePoints();
	
	for (FBranch& Branch : Tree)
	{
		Spline->SetSplinePoints(TransformsToVectors(Branch.Points), ESplineCoordinateSpace::Local);
		for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 2; i++)
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
				AddInstanceComponent(SplineMesh);

				FVector StartLocation, StartTangent, EndLocation, EndTangent;
				Spline->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
				Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);
				SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

				SplineMeshes.Add(SplineMesh);
			}
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
