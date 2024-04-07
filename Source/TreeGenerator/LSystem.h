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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Axiom = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Generation = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRule> ProductionRules;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString CurrentString;
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

	FString ApplyRule(FString c);
};
