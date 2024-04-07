// Armand Yilinkou 2024

#include "LSystem.h"

ULSystem::ULSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULSystem::Init()
{
	if (ProductionRules.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("L-system %s has no production rules!"), *GetName());
		return;
	}

	ProcessAlphabet();

	CurrentString = Axiom;

	for (int i = 0; i < Generation; i++)
	{
		Evolve();
	}
}

void ULSystem::Evolve()
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

FString ULSystem::ApplyRule(FString c)
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

void ULSystem::ProcessAlphabet()
{
	for (const FRule& Rule : ProductionRules)
	{
		Alphabet.AddUnique(Rule.Element);

		for (size_t i = 0; i < Rule.ProductionRule.Len(); i++)
		{
			FString Char = Rule.ProductionRule.Mid(i, 1);
			Alphabet.AddUnique(Char);
		}
	}
	Alphabet.Sort();
}
