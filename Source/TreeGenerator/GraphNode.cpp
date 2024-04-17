// Armand Yilinkou 2024

#include "GraphNode.h"

int FGraphNode::MaxSeenIndex = 0;

FGraphNode::FGraphNode()
{
}

TSharedPtr<FGraphNode> FGraphNode::Add()
{
	TSharedPtr<FGraphNode> ChildNode = MakeShared<FGraphNode>();
	ChildNode->ParentNode = TWeakPtr<FGraphNode>(AsShared());
	ChildNode->DistanceFromRoot = DistanceFromRoot + 1;
	MaxSeenIndex++;
	ChildNode->Index = MaxSeenIndex;
	ChildNodes.Add(ChildNode);

	return ChildNode;
}

int FGraphNode::CalculateDistanceFromTip(int MinDistFromTipForTwig)
{
	if (ChildNodes.IsEmpty())
	{
		DistanceFromFurthestTip = 0;
		bHasTwig = false;
		return DistanceFromFurthestTip;
	}

	for (const TSharedPtr<FGraphNode> Node : ChildNodes)
	{
		DistanceFromFurthestTip = std::max(DistanceFromFurthestTip, Node->CalculateDistanceFromTip(MinDistFromTipForTwig) + 1);
	}

	if (DistanceFromFurthestTip < MinDistFromTipForTwig)
	{
		bHasTwig = false;
	}

	return DistanceFromFurthestTip;
}
