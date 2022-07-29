// Copyright Epic Games, Inc. All Rights Reserved.

// TAKEN FROM UE5 EARLY ACCESS AT A STABLE POINT IN TIME - UE4.26 DOES NOT HAVE A SUFFICIENT FULL BODY IK SOLVER

#pragma once

#include "CoreMinimal.h"

#include "MIPBIKSolver.h"

#include "MIPBIK_Shared.generated.h"

UENUM(BlueprintType)
enum class EMIPBIKLimitType : uint8
{
	Free,
	Limited,
	Locked,
};

USTRUCT(BlueprintType)
struct FMIPBIKBoneSetting
{
	GENERATED_BODY()

	FMIPBIKBoneSetting()
		: Bone(NAME_None)
		, X(EMIPBIKLimitType::Free)
		, Y(EMIPBIKLimitType::Free)
		, Z(EMIPBIKLimitType::Free)
		, PreferredAngles(0.0f) {}

	UPROPERTY(meta = (Constant, CustomWidget = "BoneName"))
	FName Bone;

	UPROPERTY(meta = (ClampMin = "0", ClampMax = "1", UIMin = "0.0", UIMax = "1.0"))
	float RotationStiffness = 0.0f;
	UPROPERTY(meta = (ClampMin = "0", ClampMax = "1", UIMin = "0.0", UIMax = "1.0"))
	float PositionStiffness = 0.0f;

	UPROPERTY()
	EMIPBIKLimitType X;
	UPROPERTY(meta = (ClampMin = "-180", ClampMax = "0", UIMin = "-180.0", UIMax = "0.0"))
	float MinX = 0.0f;
	UPROPERTY(meta = (ClampMin = "0", ClampMax = "180", UIMin = "0.0", UIMax = "180.0"))
	float MaxX = 0.0f;

	UPROPERTY()
	EMIPBIKLimitType Y;
	UPROPERTY(meta = (ClampMin = "-180", ClampMax = "0", UIMin = "-180.0", UIMax = "0.0"))
	float MinY = 0.0f;
	UPROPERTY(meta = (ClampMin = "0", ClampMax = "180", UIMin = "0.0", UIMax = "180.0"))
	float MaxY = 0.0f;

	UPROPERTY()
	EMIPBIKLimitType Z;
	UPROPERTY(meta = (ClampMin = "-180", ClampMax = "0", UIMin = "-180.0", UIMax = "0.0"))
	float MinZ = 0.0f;
	UPROPERTY(meta = (ClampMin = "0", ClampMax = "180", UIMin = "0.0", UIMax = "180.0"))
	float MaxZ = 0.0f;

	UPROPERTY()
	bool bUsePreferredAngles = false;
	UPROPERTY()
	FRotator PreferredAngles;

	void CopyToCoreStruct(MIPBIK::FMIBoneSettings& Settings) const
	{
		Settings.RotationStiffness = RotationStiffness;
		Settings.PositionStiffness = PositionStiffness;
		Settings.X = static_cast<MIPBIK::EMILimitType>(X);
		Settings.MinX = MinX;
		Settings.MaxX = MaxX;
		Settings.Y = static_cast<MIPBIK::EMILimitType>(Y);
		Settings.MinY = MinY;
		Settings.MaxY = MaxY;
		Settings.Z = static_cast<MIPBIK::EMILimitType>(Z);
		Settings.MinZ = MinZ;
		Settings.MaxZ = MaxZ;
		Settings.bUsePreferredAngles = bUsePreferredAngles;
		Settings.PreferredAngles = PreferredAngles;
	}
};