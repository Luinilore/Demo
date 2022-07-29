// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "UObject/NameTypes.h"
#include "Math/Vector.h"
#include "Math/Quat.h"

struct FMIPBIKSolverSettings;

namespace MIPBIK
{

struct FMIJointConstraint;
struct FMIRigidBody;
struct FEffector;

struct FMIBone
{
	FName Name;
	int ParentIndex = -2; // -2 is unset, -1 for root, or 0...n otherwise
	bool bIsSolverRoot = false;
	bool bIsSolved = false;
	bool bIsSubRoot = false;
	FVector Position;
	FQuat Rotation;
	FVector LocalPositionOrig;
	FQuat LocalRotationOrig;

	// initialized - these fields are null/empty until after Solver::Initialize()
	FMIRigidBody* Body = nullptr;
	FMIBone* Parent = nullptr;
	TArray<FMIBone*> Children;
	// initialized

	FMIBone(
		const FName InName,
		const int& InParentIndex,		// must pass -1 for root of whole skeleton
		const FVector& InOrigPosition,
		const FQuat& InOrigRotation,
		bool bInIsSolverRoot);

	bool HasChild(const FMIBone* Bone);
};

enum class EMILimitType : uint8
{
	Free,
	Limited,
	Locked,
};

struct FMIBoneSettings
{
	float RotationStiffness = 0.0f; // range (0, 1)
	float PositionStiffness = 0.0f; // range (0, 1)

	EMILimitType X;
	float MinX = 0.0f; // range (-180, 180)
	float MaxX = 0.0f;

	EMILimitType Y;
	float MinY = 0.0f;
	float MaxY = 0.0f;

	EMILimitType Z;
	float MinZ = 0.0f;
	float MaxZ = 0.0f;

	bool bUsePreferredAngles = false;
	FRotator PreferredAngles = FRotator::ZeroRotator;
};

struct FMIRigidBody
{
	FMIBone* Bone = nullptr;
	FMIBoneSettings J;

	FVector Position;
	FQuat Rotation;
	FQuat RotationOrig;
	FVector BoneLocalPosition;
	TArray<FVector> ChildLocalPositions;

	float InvMass = 0.0f;
	FEffector* AttachedEffector = nullptr;
	float Length;
	
private:

	int NumBonesToRoot = 0;

public:

	FMIRigidBody(FMIBone* InBone);

	void Initialize(FMIBone* SolverRoot);

	void UpdateFromInputs(const FMIPBIKSolverSettings& Settings);

	int GetNumBonesToRoot() const;

	FMIRigidBody* GetParentBody();

	void ApplyPushToRotateBody(const FVector& Push, const FVector& Offset);
	
	void ApplyPushToPosition(const FVector& Push);
};

// for sorting Bodies hierarchically (root to leaf order)
inline bool operator<(const FMIRigidBody& Lhs, const FMIRigidBody& Rhs)
{ 
	return Lhs.GetNumBonesToRoot() < Rhs.GetNumBonesToRoot(); 
}

} // namespace