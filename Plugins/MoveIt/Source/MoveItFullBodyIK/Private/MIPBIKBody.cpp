// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#include "MIPBIKBody.h"
#include "MIPBIKSolver.h"

namespace MIPBIK
{

FMIBone::FMIBone(
	const FName InName,
	const int& InParentIndex,		// must pass -1 for root of whole skeleton
	const FVector& InOrigPosition,
	const FQuat& InOrigRotation,
	bool bInIsSolverRoot)
{
	Name = InName;
	ParentIndex = InParentIndex;
	Position = InOrigPosition;
	Rotation = InOrigRotation;
	bIsSolverRoot = bInIsSolverRoot;
	bIsSolved = false; // default
}

bool FMIBone::HasChild(const FMIBone* Bone)
{
	for (const FMIBone* Child : Children)
	{
		if (Bone->Name == Child->Name)
		{
			return true;
		}
	}

	return false;
}

FMIRigidBody::FMIRigidBody(FMIBone* InBone)
{
	Bone = InBone;
	J = FMIBoneSettings();
}

void FMIRigidBody::Initialize(FMIBone* SolverRoot)
{
	FVector Centroid = Bone->Position;
	Length = 0.0f;
	for(const FMIBone* Child : Bone->Children)
	{
		Centroid += Child->Position;
		Length += (Bone->Position - Child->Position).Size();
	}
	Centroid = Centroid * (1.0f / ((float)Bone->Children.Num() + 1.0f));

	Position = Centroid;
	Rotation = RotationOrig = Bone->Rotation;
	BoneLocalPosition = Rotation.Inverse() * (Bone->Position - Centroid);

	for (FMIBone* Child : Bone->Children)
	{
		FVector ChildLocalPos = Rotation.Inverse() * (Child->Position - Centroid);
		ChildLocalPositions.Add(ChildLocalPos);
	}

	// calculate num bones distance to root
	NumBonesToRoot = 0;
	FMIBone* Parent = Bone;
	while (Parent && Parent != SolverRoot)
	{
		NumBonesToRoot += 1;
		Parent = Parent->Parent;
	}
}

void FMIRigidBody::UpdateFromInputs(const FMIPBIKSolverSettings& Settings)
{
	// set to input pose
	Position = Bone->Position - Bone->Rotation * BoneLocalPosition;
	Rotation = Bone->Rotation;

	// Body.Length used as rough approximation of the mass of the body
	// for fork joints (multiple solved children) we sum lengths to all children (see Initialize)
	InvMass = 1.0f / ( Length * ((Settings.MassMultiplier * GLOBAL_UNITS) + 0.5f));
}

int FMIRigidBody::GetNumBonesToRoot() const
{ 
	return NumBonesToRoot; 
}

FMIRigidBody* FMIRigidBody::GetParentBody()
{
	if (Bone && Bone->Parent)
	{
		return Bone->Parent->Body;
	}

	return nullptr;
}

void FMIRigidBody::ApplyPushToRotateBody(const FVector& Push, const FVector& Offset)
{
	// equation 8 in "Detailed Rigid Body Simulation with XPBD"
	FVector Omega = InvMass * (1.0f - J.RotationStiffness) * FVector::CrossProduct(Offset, Push);
	FQuat OQ(Omega.X, Omega.Y, Omega.Z, 0.0f);
	OQ = OQ * Rotation;
	OQ.X *= 0.5f;
	OQ.Y *= 0.5f;
	OQ.Z *= 0.5f;
	OQ.W *= 0.5f;
	Rotation.X = Rotation.X + OQ.X;
	Rotation.Y = Rotation.Y + OQ.Y;
	Rotation.Z = Rotation.Z + OQ.Z;
	Rotation.W = Rotation.W + OQ.W;
	Rotation.Normalize();
}

void FMIRigidBody::ApplyPushToPosition(const FVector& Push)
{
	if (AttachedEffector)
	{
		return; // pins are locked
	}

	Position += Push * (1.0f - J.PositionStiffness);
}

} // namespace