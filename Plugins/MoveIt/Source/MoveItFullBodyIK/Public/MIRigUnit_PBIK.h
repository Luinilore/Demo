// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "Units/Highlevel/RigUnit_HighlevelBase.h"
#include "Drawing/ControlRigDrawInterface.h"

#include "MIPBIKSolver.h"
#include "MIPBIKDebug.h"

#include "MIPBIK_Shared.h"

#include "MIRigUnit_PBIK.generated.h"

using MIPBIK::FMIDebugLine;

USTRUCT()
struct FMIPBIKDebug
{
	GENERATED_BODY()

	UPROPERTY()
	float DrawScale = 1.0f;

	UPROPERTY()
	bool bDrawDebug = false;

	void Draw(FControlRigDrawInterface* DrawInterface, FMIPBIKSolver* Solver) const
	{
		if (!(DrawInterface && Solver && bDrawDebug))
		{
			return;
		}

		const FLinearColor Bright = FLinearColor(0.f, 1.f, 1.f, 1.f);
		DrawInterface->DrawBox(FTransform::Identity, FTransform(FQuat::Identity, FVector(0, 0, 0), FVector(1.f, 1.f, 1.f) * DrawScale * 0.1f), Bright);

		TArray<FMIDebugLine> BodyLines;
		Solver->GetDebugDraw()->GetDebugLinesForBodies(BodyLines);
		const FLinearColor BodyColor = FLinearColor(0.1f, 0.1f, 1.f, 1.f);
		for (FMIDebugLine Line : BodyLines)
		{
			DrawInterface->DrawLine(FTransform::Identity, Line.A, Line.B, BodyColor);
		}
	}
};

USTRUCT(BlueprintType)
struct FMIPBIKEffector
{
	GENERATED_BODY()

	FMIPBIKEffector()	: Bone(NAME_None) {}

	UPROPERTY(meta = (Constant, CustomWidget = "BoneName"))
	FName Bone;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	float OffsetAlpha = 1.0f;

	UPROPERTY()
	float StrengthAlpha = 1.0f;
};

USTRUCT(meta=(DisplayName="MoveIt Full Body IK", Category="Hierarchy", Keywords="Position Based, PBIK, IK, Full Body, Multi, Effector, N-Chain, FB. Taken from UE5 Early Access."))
struct FMIRigUnit_PBIK : public FRigUnit_HighlevelBaseMutable
{
	GENERATED_BODY()

	RIGVM_METHOD()
	virtual void Execute(const FRigUnitContext& Context) override;

	FMIRigUnit_PBIK()
		: Root(NAME_None)
	{
	}

	UPROPERTY(meta = (Input, Constant, CustomWidget = "BoneName"))
	FName Root;

	UPROPERTY(meta = (Input))
	TArray<FMIPBIKEffector> Effectors;
	UPROPERTY(transient)
	TArray<int32> EffectorSolverIndices;

	UPROPERTY(meta = (Input))
	TArray<FMIPBIKBoneSetting> BoneSettings;

	UPROPERTY(meta = (Input))
	FMIPBIKSolverSettings Settings;

	UPROPERTY(meta = (Input))
	FMIPBIKDebug Debug;

	UPROPERTY(transient)
	FMIPBIKSolver Solver;
};
