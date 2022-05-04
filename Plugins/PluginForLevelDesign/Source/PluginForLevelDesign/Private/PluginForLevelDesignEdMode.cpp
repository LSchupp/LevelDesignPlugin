// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginForLevelDesignEdMode.h"
#include "PluginForLevelDesignEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FPluginForLevelDesignEdMode::EM_PluginForLevelDesignEdModeId = TEXT("EM_PluginForLevelDesignEdMode");

FPluginForLevelDesignEdMode::FPluginForLevelDesignEdMode()
{

}

FPluginForLevelDesignEdMode::~FPluginForLevelDesignEdMode()
{

}

void FPluginForLevelDesignEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FPluginForLevelDesignEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FPluginForLevelDesignEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FPluginForLevelDesignEdMode::UsesToolkits() const
{
	return true;
}




