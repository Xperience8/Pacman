// Copyright 2015 Patrick Jurasek. All Rights Reserved.

#pragma once

#include "Object.h"
#include "PacmanNavigationSystem.generated.h"

class PacmanNavigationConnection;

/**
* 
*/
class PACMAN_API PacmanNavigationNode
{
public:
	static const float NODE_RADIUS;
	static const float NODE_RADIUS_SQUARED;

	PacmanNavigationNode(FVector NodeLocation) : Location(NodeLocation) { }
	~PacmanNavigationNode() { }

	void AddConnection(const PacmanNavigationConnection &Connection)
	{
		Connections.Add(Connection);
	}

	bool IsIn(FVector TestLocation) const
	{
		return (TestLocation - Location).SizeSquared2D() < NODE_RADIUS_SQUARED;
	}

	const FVector &GetLocation() const
	{
		return Location;
	}

	const TArray<PacmanNavigationConnection> &GetConnections() const
	{
		return Connections;
	}

private:
	FVector Location;

	TArray<PacmanNavigationConnection> Connections;
};

/**
*
*/
class PACMAN_API PacmanNavigationConnection
{
public:
	PacmanNavigationConnection(PacmanNavigationNode *Start, PacmanNavigationNode *Goal, float ConnectionCost) : Head(Start), Tail(Goal), Cost(ConnectionCost)
	{
		check(Head && Tail);

		//UE_LOG(LogTemp, Warning, TEXT("Path from %f %f %f to %f %f %f was created"), Head->GetLocation().X, Head->GetLocation().Y, Head->GetLocation().Z, Tail->GetLocation().X, Tail->GetLocation().Y, Tail->GetLocation().Z);
	}
	~PacmanNavigationConnection() { }

	PacmanNavigationNode *GetHead() const
	{
		return Head;
	}

	PacmanNavigationNode *GetTail() const
	{
		return Tail;
	}

	float GetCost() const
	{
		return Cost;
	}

private:
	PacmanNavigationNode *Head;

	PacmanNavigationNode *Tail;

	float Cost;
};

/**
*
*/
class PACMAN_API PathPlan
{
public:
	///////////////////////////////////////////////////////////////////////
	///                      Reworked                                   ///
	///////////////////////////////////////////////////////////////////////

public:
	PathPlan() : GoalNode(FVector(0.f, 0.f, 0.f)), CurrentSegmentId(0) { }
	~PathPlan() { }

	void Destroy()
	{
		Segments.Empty();

		CurrentSegmentId = 0;
	}
	
	void PostInit(FVector GoalLocation)
	{
		if (Segments.Num() < 1)
		{
			GoalNode = PacmanNavigationNode(GoalLocation);
			Segments.Add(&GoalNode);
			return;
		}

		float Tolerance = 10.f;
		if ((GoalLocation - Segments.Last()->GetLocation()).Size2D() < Tolerance)
		{
			auto LastNode = Segments.Last();
			Segments.Pop(false);
			PostInit(LastNode);
		}
		else
		{
			GoalNode = PacmanNavigationNode(GoalLocation);

			FVector OffsetToGoalNode = GoalLocation - Segments.Last()->GetLocation();
			FVector DirToGoalNode = OffsetToGoalNode.GetUnsafeNormal();
			for (auto &Connection : Segments.Last()->GetConnections())
			{
				FVector OffsetToNextNode = Connection.GetTail()->GetLocation() - GoalLocation;
				FVector DirToNextNode = OffsetToNextNode.GetUnsafeNormal();
				if (FVector::DotProduct(DirToGoalNode, DirToNextNode) > 0.5f)
				{
					GoalNode.AddConnection(PacmanNavigationConnection(&GoalNode, Connection.GetTail(), OffsetToNextNode.Size2D()));
				}
			}

			GoalNode.AddConnection(PacmanNavigationConnection(&GoalNode, Segments.Last(), OffsetToGoalNode.Size2D()));

			Segments.Add(&GoalNode);
		}
	}

	void PostInit(PacmanNavigationNode *NewGoalNode)
	{
		GoalNode = *NewGoalNode;
		Segments.Add(&GoalNode);
	}


	bool IsAtGoal() const
	{
		return CurrentSegmentId == Segments.Num();
	}

	bool CheckForNextSegment(FVector CurrentLocation)
	{
		const float Tolerance = 20.f;
		float Distance = (GetCurrentSegment()->GetLocation() - CurrentLocation).Size2D();
		if (Distance < Tolerance)
		{
			++CurrentSegmentId;
			return true;
		}
		return false;
	}




	void AddSegment(PacmanNavigationNode *Segment)
	{
		Segments.Add(Segment);
	}

	void MoveToNextSegment()
	{
		++CurrentSegmentId;
	}



	PacmanNavigationNode *GetCurrentSegment() const
	{
		return Segments[CurrentSegmentId];
	}
	PacmanNavigationNode *GetGoalSegment()
	{
		return &GoalNode;
	}
	
	const TArray<PacmanNavigationNode*> &GetSegments() const
	{
		return Segments;
	}

private:
	PacmanNavigationNode GoalNode;
	
	TArray<PacmanNavigationNode*> Segments;
	int32 CurrentSegmentId;

	///////////////////////////////////////////////////////////////////////
	///                      Reworked                                   ///
	///////////////////////////////////////////////////////////////////////
};

/**
*
*/
class PACMAN_API PathPlanNode
{
public:
	PathPlanNode(PacmanNavigationNode *InNode, PathPlanNode *InPrevNode, float InCostSoFar) : Node(InNode), PrevNode(InPrevNode), CostSoFar(InCostSoFar), Closed(false) { };
	~PathPlanNode() { }

	void Close()
	{
		Closed = true;
	}

	bool IsClosed() const
	{
		return Closed;
	}

	void SetCost(float NewCost)
	{
		CostSoFar = NewCost;
	}
	void SetPrevNode(PathPlanNode *NewPrevNode)
	{
		PrevNode = NewPrevNode;
	}

	float GetCost() const
	{
		return CostSoFar;
	}
	PacmanNavigationNode *GetNode() const
	{
		return Node;
	}
	PathPlanNode *GetPrevNode() const
	{
		return PrevNode;
	}
private:
	PacmanNavigationNode *Node;
	PathPlanNode *PrevNode;

	float CostSoFar;

	bool Closed;
};

/**
*
*/
USTRUCT()
struct PACMAN_API FPacmanNavigationData
{
	GENERATED_USTRUCT_BODY()

	void Connect(PacmanNavigationNode &Head, PacmanNavigationNode &Tail)
	{
		Head.AddConnection(PacmanNavigationConnection(&Head, &Tail, (Tail.GetLocation() - Head.GetLocation()).Size()));
		Tail.AddConnection(PacmanNavigationConnection(&Tail, &Head, (Head.GetLocation() - Tail.GetLocation()).Size()));
	}

	void Init(int32 MaxNumNodes = 1024)
	{
		NavNodes.Empty();
		NavNodes.Reserve(MaxNumNodes);
	}

	TArray<PacmanNavigationNode> NavNodes;
};

/**
 * 
 */
UCLASS()
class PACMAN_API UPacmanNavigationSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UPacmanNavigationSystem() { }

	/// Find the shortest path to destination location(Dijkstra algorithm)
	void FindPathToLocation(FVector CurrentLocation, FVector Destination, FVector ForwardVector, PathPlan &Path);

	/// Only one navigation data can be active at one time
	void SetNavigationData(FPacmanNavigationData *NavigationData)
	{
		NavData = NavigationData;
	}

	/// Returns node that is the nearest node to location and it checks whether
	/// location lies in node's radius or in any connection from found node
	/// If there isn't any node which accomplish this conditions, then GetNearedNodeRoughly is called
	PacmanNavigationNode *GetNearestNode(FVector Location);

	/// Returns node that is the nearest node to location, but doesn't check whether
	/// this location actually lies on any connection, from this node or in nodes area
	PacmanNavigationNode *GetNearestNodeRoughly(FVector Location);

private:
	void FinalizePath(PathPlanNode *GoalPathPlanNode, FVector StartLocation, FVector GoalLocation, PathPlan &Path);

	/// Checks whether location is between nodes(on their's connection)
	bool LieBetweenNodes(FVector Location, PacmanNavigationNode *Node1, PacmanNavigationNode *Node2)
	{
		float SquaredCost = (Node2->GetLocation() - Node1->GetLocation()).SizeSquared2D();
		float SquaredDistanceFrom1Node = (Node1->GetLocation() - Location).SizeSquared2D();
		float SquaredDistanceFrom2Node = (Node2->GetLocation() - Location).SizeSquared2D();

		return (SquaredDistanceFrom1Node < SquaredCost && SquaredDistanceFrom2Node < SquaredCost);
	}

	FPacmanNavigationData *NavData;
};

extern PACMAN_API UPacmanNavigationSystem *GNavigationSystem;
