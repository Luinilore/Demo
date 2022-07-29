// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#include "MIPBIKDebug.h"
#include "MIPBIKSolver.h"
#include "MIPBIKBody.h"

namespace MIPBIK
{
	void FMIDebugDraw::GetDebugLinesForBodies(TArray<FMIDebugLine>& OutLines)
	{
		OutLines.Empty();
		for (const FMIRigidBody& Body : Solver->Bodies)
		{
			if (Body.Bone->Children.Num() == 1)
			{
				FVector StartPos = Body.Position + Body.Rotation * Body.BoneLocalPosition;
				FVector EndPos = Body.Position + Body.Rotation * Body.ChildLocalPositions[0];
				OutLines.Emplace(StartPos, EndPos);
			}
		}
	}
}
