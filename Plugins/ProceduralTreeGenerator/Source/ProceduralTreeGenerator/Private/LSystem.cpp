// Armand Yilinkou 2024

#include "LSystem.h"

#include "Timer.h"

ULSystem::ULSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULSystem::Init()
{
	// Timer FunctionTimer("ULSystem::Init()");
	
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
		FString Char = CurrentString.Mid(i, 1);
		if (Char == "+" || Char == "-" || Char == "[" || Char == "]")
		{
			NextString.Append(Char);
			continue;
		}
		NextString.Append(ApplyRule(Char));
	}
	CurrentString = NextString;
}

FString ULSystem::ApplyRule(FString Character)
{	
	for (const FRule& Rule : ProductionRules)
	{
		if (Rule.Element == Character)
		{
			return Rule.ProductionRule;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find rule for: %s"), *Character);
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
