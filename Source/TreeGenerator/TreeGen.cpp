// Armand Yilinkou 2024


#include "TreeGen.h"

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
