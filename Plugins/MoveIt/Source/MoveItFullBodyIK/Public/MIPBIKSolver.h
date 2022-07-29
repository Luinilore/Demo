// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "MIPBIKBody.h"
#include "MIPBIKConstraint.h"
#include "MIPBIKDebug.h"

#include "MIPBIKSolver.generated.h"

DECLARE_CYCLE_STAT(TEXT("PBIK Solve"), STAT_PBIK_Solve, STATGROUP_Anim);

namespace MIPBIK
{

static float GLOBAL_UNITS = 100.0f; // (1.0f = meters), (100.0f = centimeters)

struct FMIRigidBody;

struct FEffector
{
	FVector Position;
	FQuat Rotation;

	FVector PositionOrig;
	FQuat RotationOrig;

	FVector PositionGoal;
	FQuat RotationGoal;

	FMIBone* Bone;
	TWeakPtr<FMIPinConstraint> Pin;
	
	float DistToSubRootOrig;
	FMIRigidBody* ParentSubRoot = nullptr;

	float TransformAlpha;
	float StrengthAlpha;

	FEffector(FMIBone* InBone);

	void SetGoal(const FVector InPositionGoal, const FQuat& InRotationGoal, float InTransformAlpha, float InStrengthAlpha);

	void UpdateFromInputs();

	void SquashSubRoots();
};

} // namespace

USTRUCT()
struct FMIPBIKSolverSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = SolverSettings, meta = (ClampMin = "0", UIMin = "0.0", UIMax = "200.0"))
	int32 Iterations = 20;

	UPROPERTY(EditAnywhere, Category = SolverSettings, meta = (ClampMin = "0", UIMin = "0.0", UIMax = "10.0"))
	float MassMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = SolverSettings)
	bool bAllowStretch = false;

	UPROPERTY(EditAnywhere, Category = SolverSettings)
	bool bPinRoot = false;
};



USTRUCT()
struct FMIPBIKSolver
{
	GENERATED_BODY()

public:

	MIPBIK::FMIDebugDraw* GetDebugDraw();

	//
	// main runtime functions
	//

	bool Initialize();

	void Solve(const FMIPBIKSolverSettings& Settings);

	void Reset();

	bool IsReadyToSimulate();

	//
	// set input / get output at runtime
	//

	void SetBoneTransform(const int32 Index, const FTransform& InTransform);

	MIPBIK::FMIBoneSettings* GetBoneSettings(const int32 Index);

	void SetEffectorGoal(
		const int32 Index, 
		const FVector& InPosition, 
		const FQuat& InRotation, 
		const float OffsetAlpha, 
		const float StrengthAlpha);

	void GetBoneGlobalTransform(const int32 Index, FTransform& OutTransform);

	//
	// pre-init /  setup functions
	//

	int32 AddBone(
		const FName Name,
		const int32 ParentIndex,
		const FVector& InOrigPosition,
		const FQuat& InOrigRotation,
		bool bIsSolverRoot);

	int32 AddEffector(FName BoneName);
	
private:

	bool InitBones();

	bool InitBodies();

	void InitConstraints();

	void AddBodyForBone(MIPBIK::FMIBone* Bone);

private:

	MIPBIK::FMIBone* SolverRoot = nullptr;
	TWeakPtr<MIPBIK::FMIPinConstraint> RootPin = nullptr;
	TArray<MIPBIK::FMIBone> Bones;
	TArray<MIPBIK::FMIRigidBody> Bodies;
	TArray<TSharedPtr<MIPBIK::FMIConstraint>> Constraints;
	TArray<MIPBIK::FEffector> Effectors;
	bool bReadyToSimulate = false;
	
	MIPBIK::FMIDebugDraw DebugDraw;
	friend MIPBIK::FMIDebugDraw;
};
