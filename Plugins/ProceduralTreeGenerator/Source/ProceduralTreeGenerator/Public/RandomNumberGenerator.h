// Armand Yilinkou 2024

#pragma once

#include <random>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RandomNumberGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURALTREEGENERATOR_API URandomNumberGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	URandomNumberGenerator();

	UFUNCTION(BlueprintCallable)
	void Init();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RandomSeed = 1;

private:
	int Max = 999999;
	
	std::default_random_engine Engine;
	std::uniform_int_distribution<int> Distribution{ 0, Max };

public:
	UFUNCTION(BlueprintCallable)
	int GenerateNumber();
	
	UFUNCTION(BlueprintCallable)
	int GetMax() const;
};
