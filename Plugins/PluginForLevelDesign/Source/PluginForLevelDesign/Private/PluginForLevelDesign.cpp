// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginForLevelDesign.h"
#include "PluginForLevelDesignEdMode.h"

#define LOCTEXT_NAMESPACE "FPluginForLevelDesignModule"

void FPluginForLevelDesignModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FPluginForLevelDesignEdMode>(FPluginForLevelDesignEdMode::EM_PluginForLevelDesignEdModeId, LOCTEXT("PluginForLevelDesignEdModeName", "Level design Tool"), FSlateIcon(), true);
}

void FPluginForLevelDesignModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FPluginForLevelDesignEdMode::EM_PluginForLevelDesignEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPluginForLevelDesignModule, PluginForLevelDesign)