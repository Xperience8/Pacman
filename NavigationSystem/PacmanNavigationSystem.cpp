// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#include "Pacman.h"
#include "PacmanNavigationSystem.h"

const float PacmanNavigationNode::NODE_RADIUS = 50.f;
const float PacmanNavigationNode::NODE_RADIUS_SQUARED = PacmanNavigationNode::NODE_RADIUS * PacmanNavigationNode::NODE_RADIUS;

UPacmanNavigationSystem *GNavigationSystem;

template<>
struct TLess<PathPlanNode*>
{
	FORCEINLINE bool operator()(const PathPlanNode &A, const PathPlanNode &B) const
	{
		return A.GetCost() < B.GetCost();
	}
};

void UPacmanNavigationSystem::FindPathToLocation(FVector CurrentLocation, FVector Destination, FVector ForwardVector, PathPlan &Path)
{
	Path.Destroy();

	TMap<PacmanNavigationNode*, PathPlanNode> NodeToPathPlanNodeMap;
	TArray<PathPlanNode*> OpenList;

	NodeToPathPlanNodeMap.Reserve(100);

	auto Start = GetNearestNode(CurrentLocation);
	auto Goal = GetNearestNode(Destination);

	/// Every location on the map should have corresponding node 
	check(Start || Goal);

	if (Start == Goal)
	{
		// In Pacman ghost must always move, therefore if we are at goal location,
		// then new path from goal location to goal location is created(no backward moving)
		if ((CurrentLocation - Destination).SizeSquared2D() < PacmanNavigationNode::NODE_RADIUS_SQUARED)
		{
			for (auto &Connection : Start->GetConnections())
			{
				// Offset start node to next node, which is directly before start node
				FVector DirToNode = (Connection.GetTail()->GetLocation() - CurrentLocation).GetUnsafeNormal();
				if (FVector::DotProduct(DirToNode, ForwardVector) > -0.75f)
				{
					Start = Connection.GetTail();
					ForwardVector = DirToNode;
					break;
				}
			}
		}
		// Move directly to target
		else
		{
			Path.PostInit(Destination);
		}
	}

	// First move is really important because ghost can not move backward, therefore 
	// nodes which are in backward direction are skipped
	auto CurrentPathPlanNode = &NodeToPathPlanNodeMap.Emplace(Start, PathPlanNode(Start, nullptr, 0.f));
	{
		CurrentPathPlanNode->Close();

		auto Connections = CurrentPathPlanNode->GetNode()->GetConnections();
		for (int32 i = 0; i < Connections.Num(); ++i)
		{
			// Block nodes which are in backward direction
			FVector DirToNewNode = (Connections[i].GetTail()->GetLocation() - Connections[i].GetHead()->GetLocation()).GetUnsafeNormal();
			if (FVector::DotProduct(ForwardVector, DirToNewNode) > -0.75f)
			{
				auto Node = &NodeToPathPlanNodeMap.Emplace(Connections[i].GetTail(), PathPlanNode(Connections[i].GetTail(), CurrentPathPlanNode, CurrentPathPlanNode->GetCost() + Connections[i].GetCost()));
				OpenList.HeapPush(Node);
			}
		}
	}

	while (OpenList.Num() > 0)
	{
		OpenList.HeapPop(CurrentPathPlanNode);

		if (CurrentPathPlanNode->GetNode() == Goal)
		{
			break;
		}

		CurrentPathPlanNode->Close();

		auto Connections = CurrentPathPlanNode->GetNode()->GetConnections();
		for (int32 i = 0; i < Connections.Num(); ++i)
		{
			auto Node = NodeToPathPlanNodeMap.Find(Connections[i].GetTail());
			if (Node)
			{
				// If node is closed then we know that this is a bad direction
				if (Node->IsClosed())
				{
					continue;
				}
				// If new path has lower cost => save it
				else if (Node->GetCost() > CurrentPathPlanNode->GetCost() + Connections[i].GetCost())
				{
					Node->SetCost(CurrentPathPlanNode->GetCost() + Connections[i].GetCost());
					Node->SetPrevNode(CurrentPathPlanNode);
				}
			}
			else
			{
				Node = &NodeToPathPlanNodeMap.Emplace(Connections[i].GetTail(), PathPlanNode(Connections[i].GetTail(), CurrentPathPlanNode, CurrentPathPlanNode->GetCost() + Connections[i].GetCost()));
				//Node = &NodeToPathPlanNodeMap.Add(Connections[i].GetTail(), PathPlanNode(Connections[i].GetTail(), CurrentPathPlanNode, CurrentPathPlanNode->GetCost() + Connections[i].GetCost()));
				OpenList.HeapPush(Node);
			}
		}
	}

	FinalizePath(CurrentPathPlanNode, CurrentLocation, Destination, Path);
}

PacmanNavigationNode * UPacmanNavigationSystem::GetNearestNode(FVector Location)
{
	check(NavData);

	PacmanNavigationNode *NearestNode = nullptr;
	float SquaredDistanceToNearestNode = FLT_MAX;

	for (auto &Node : NavData->NavNodes)
	{
		float SquaredDistanceToNode = (Node.GetLocation() - Location).SizeSquared2D();
		if (SquaredDistanceToNode < SquaredDistanceToNearestNode)
		{
			// If location is inside node's radius then it is guaranteed that there won't be any node which is closed
			if (SquaredDistanceToNode < PacmanNavigationNode::NODE_RADIUS_SQUARED)
			{
				NearestNode = &Node;
				break;
			}

			// Check if location lies on any connection from selected node
			for (auto &Connection : Node.GetConnections())
			{
				if (LieBetweenNodes(Location, Connection.GetHead(), Connection.GetTail()))
				{
					SquaredDistanceToNearestNode = SquaredDistanceToNode;
					NearestNode = &Node;
				}
			}
		}
	}

	// If we didn't find any node that is close to location, therefore try to find it roughly(based on distance, no checking)
	if (!NearestNode)
	{
		NearestNode = GetNearestNodeRoughly(Location);
	}

	return NearestNode;
}

PacmanNavigationNode * UPacmanNavigationSystem::GetNearestNodeRoughly(FVector Location)
{
	check(NavData);

	PacmanNavigationNode *NearestNode = nullptr;
	float SquaredDistanceToNearestNode = FLT_MAX;

	for (auto &Node : NavData->NavNodes)
	{
		float SquaredDistanceToNode = (Node.GetLocation() - Location).SizeSquared2D();
		if (SquaredDistanceToNode < SquaredDistanceToNearestNode)
		{
			SquaredDistanceToNearestNode = SquaredDistanceToNode;
			NearestNode = &Node;

			// If location is inside node's radius then it is guaranteed that there won't be any node which is closed
			if (SquaredDistanceToNode < PacmanNavigationNode::NODE_RADIUS_SQUARED)
			{
				break;
			}
		}
	}

	return NearestNode;
}

void UPacmanNavigationSystem::FinalizePath(PathPlanNode *GoalPathPlanNode, FVector StartLocation, FVector GoalLocation, PathPlan &Path)
{
	if (GoalPathPlanNode)
	{
		TArray<PacmanNavigationNode*> PathNodes;
		while (GoalPathPlanNode)
		{
			PathNodes.Add(GoalPathPlanNode->GetNode());
			GoalPathPlanNode = GoalPathPlanNode->GetPrevNode();
		}

		check(PathNodes.Num() > 1);

		// Start node could be before StartLocation and ghost can not go back, therefore we have to skip it
		// StartLocation could be directly inside StartNode therefore we have to skip it
		bool TakeStartNode = !PathNodes.Last()->IsIn(StartLocation) && !LieBetweenNodes(StartLocation, PathNodes.Last(), PathNodes.Last(1));
		
		// The last node could be after GoalLocation, therefore we don't use it
		// or goal location could be directly inside GoalNode, therefore we have to skip it because
		// new Goal node will be created at the end in the PostInit call
		bool TakeLastNode = !PathNodes[0]->IsIn(GoalLocation) && !LieBetweenNodes(GoalLocation, PathNodes[0], PathNodes[1]);

		// Skip nodes and reverse them
		int32 StartPathIndex = TakeStartNode ? PathNodes.Num() - 1 : PathNodes.Num() - 2;
		int32 LastPathIndex = TakeLastNode ? 0 : 1;
		for (int32 i = StartPathIndex; i >= LastPathIndex; --i)
		{
			Path.AddSegment(PathNodes[i]);
		}

		Path.PostInit(GoalLocation);
	}
}
