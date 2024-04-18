// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rule.h"
#include "LSystem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TREEGENERATOR_API ULSystem : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	ULSystem();

	UFUNCTION(BlueprintCallable)
	void Init();

public:
	// String to begin with at generation 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Axiom = "";

	// Number of generations to process string
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Generation = 1;

	// Set of production rules
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRule> ProductionRules;

	// Resulting string after all generations have been processed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString CurrentString;

	// Set of characters used
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Alphabet;

public:
	UFUNCTION(BlueprintPure)
	FString GetAxiom() const { return Axiom; }
	UFUNCTION(BlueprintPure)
	int GetGeneration() const { return Generation; }
	UFUNCTION(BlueprintPure)
	const TArray<FRule>& GetProductionRules() const { return ProductionRules; }
	UFUNCTION(BlueprintPure)
	FString GetCurrentString() const { return CurrentString; }
	UFUNCTION(BlueprintPure)
	const TArray<FString>& GetAlphabet() const { return Alphabet; }

private:
	void Evolve();
	void ProcessAlphabet();

	FString ApplyRule(FString Character);
};
