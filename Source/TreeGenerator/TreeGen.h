// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "LSystem.h"
#include "Branch.h"
#include "TreeGen.generated.h"

UCLASS()
class TREEGENERATOR_API ATreeGen : public AActor
{
	GENERATED_BODY()

	ATreeGen();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Turtle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULSystem* LSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Angle = 25.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentAngle = 0.f;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TArray<FBranch> Tree;

public:
	UFUNCTION(BlueprintCallable)
	void GenerateTree();
};
