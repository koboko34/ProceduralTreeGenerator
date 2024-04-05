// Armand Yilinkou 2024

#include "LSystem.h"

ALSystem::ALSystem()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ALSystem::Init()
{
	if (ProductionRules.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("L-system %s has no production rules!"), *GetName());
		return;
	}

	CurrentString = Axiom;
	CurrentAngle = Angle;

	for (int i = 0; i < Generation; i++)
	{
		Evolve();
	}
}

void ALSystem::Evolve()
{	
	FString NextString = "";

	for (int i = 0; i < CurrentString.Len(); i++)
	{
		FString c = CurrentString.Mid(i, 1);
		if (c == "+" || c == "-" || c == "[" || c == "]")
		{
			NextString.Append(c);
			continue;
		}
		NextString.Append(ApplyRule(c));
	}
	CurrentString = NextString;
}

FString ALSystem::ApplyRule(FString c)
{	
	for (const FRule& Rule : ProductionRules)
	{
		if (Rule.Element == c)
		{
			return Rule.ProductionRule;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find rule for: %s"), *c);
	return "?";
}
