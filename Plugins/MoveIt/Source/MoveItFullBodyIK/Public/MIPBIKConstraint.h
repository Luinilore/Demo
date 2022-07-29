// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "MIPBIKBody.h"

namespace MIPBIK
{

struct FMIConstraint
{
	bool bEnabled = true;

	virtual void Solve(bool bMoveSubRoots) = 0;
	virtual void RemoveStretch() {};
};

struct FMIJointConstraint : public FMIConstraint
{

private:

	FMIRigidBody* A;
	FMIRigidBody* B;

	FVector PinPointLocalToA;
	FVector PinPointLocalToB;

	FVector XOrig;
	FVector YOrig;
	FVector ZOrig;

	FVector XA;
	FVector YA;
	FVector ZA;
	FVector XB;
	FVector YB;
	FVector ZB;

	FVector ZBProjOnX;
	FVector ZBProjOnY;
	FVector YBProjOnZ;

	float AngleX;
	float AngleY;
	float AngleZ;

public:

	FMIJointConstraint(FMIRigidBody* InA, FMIRigidBody* InB);

	virtual ~FMIJointConstraint() {};

	virtual void Solve(bool bMoveSubRoots) override;

	virtual void RemoveStretch() override;

private:

	FVector GetPositionCorrection(FVector& OutBodyToA, FVector& OutBodyToB);

	void ApplyRotationCorrection(FQuat PureRotA, FQuat PureRotB);

	void UpdateJointLimits();

	void RotateWithinLimits(
		float MinAngle,
		float MaxAngle,
		float CurrentAngle,
		FVector RotAxis,
		FVector CurVec,
		FVector RefVec);

	void UpdateLocalRotateAxes(bool bX, bool bY, bool bZ);

	void DecomposeRotationAngles();

	float SignedAngleBetweenNormals(
		const FVector& From,
		const FVector& To,
		const FVector& Axis) const;
};

struct FMIPinConstraint : public FMIConstraint
{
	FVector GoalPoint;
	FMIRigidBody* A;
	FVector PinPointLocalToA;
	float Alpha = 1.0;

public:

	FMIPinConstraint(FMIRigidBody* InBody, const FVector& InPinPoint);

	virtual ~FMIPinConstraint() {};

	virtual void Solve(bool bMoveSubRoots) override;

private:

	FVector GetPositionCorrection(FVector& OutBodyToPinPoint) const;
};

} // namespace

