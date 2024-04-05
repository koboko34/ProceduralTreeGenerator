// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rule.h"
#include "LSystem.generated.h"

UCLASS()
class TREEGENERATOR_API ALSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSystem();

	UFUNCTION(BlueprintCallable)
	void Init();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Axiom = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Generation = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRule> ProductionRules;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString CurrentString;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentAngle;

private:
	void Evolve();

	FString ApplyRule(FString c);
};
