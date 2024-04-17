// Armand Yilinkou 2024

#pragma once

#include "CoreMinimal.h"
#include "Twig.h"
#include "Templates/SharedPointer.h"

class FGraphNode : public TSharedFromThis<FGraphNode>
{
public:
	FGraphNode();

public:
	static int MaxSeenIndex;
	
	int Index = 0;
	int DistanceFromRoot = 0;
	int DistanceFromFurthestTip = 0;
	
	bool bHasTwig = false;
	FTwig Twig;

	TArray<TSharedPtr<FGraphNode>> ChildNodes;
	TWeakPtr<FGraphNode> ParentNode;

public:
	TSharedPtr<FGraphNode> Add();

	int CalculateDistanceFromTip(int MinDistFromTipForTwig); // also disables twigs on branch tips

};