// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "Math/Vector.h"

struct FMIPBIKSolver;

namespace MIPBIK
{
	struct FMIDebugLine
	{
		FVector A;
		FVector B;

		FMIDebugLine(const FVector& InA, const FVector& InB)
		{
			A = InA;
			B = InB;
		}
	};

	struct FMIDebugDraw
	{
		FMIPBIKSolver* Solver;

		void GetDebugLinesForBodies(TArray<FMIDebugLine>& OutLines);
	};
}
