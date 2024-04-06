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
