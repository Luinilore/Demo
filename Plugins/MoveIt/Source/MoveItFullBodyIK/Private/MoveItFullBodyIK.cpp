// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.

#include "MoveItFullBodyIK.h"

#define LOCTEXT_NAMESPACE "FMoveItFullBodyIKModule"

void FMoveItFullBodyIKModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMoveItFullBodyIKModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMoveItFullBodyIKModule, MoveItFullBodyIK)

DEFINE_LOG_CATEGORY(LogMoveItIKSolver);
