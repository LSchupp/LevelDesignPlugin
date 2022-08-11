// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "DetailLayoutBuilder.h"
#include "Framework/SlateDelegates.h"

enum PivotType
{
	RIGHT_UP_UPPER,
	UP_MIDDLE_UPPER,
	LEFT_UP_UPPER,
	LEFT_MIDDLE_UPPER,
	LEFT_DOWN_UPPER,
	DOWN_MIDDLE_UPPER,
	RIGHT_DOWN_UPPER,
	RIGHT_MIDDLE_UPPER,
	CENTER_UPPER,
	RIGHT_UP,
	UP_MIDDLE,
	LEFT_UP,
	LEFT_MIDDLE,
	LEFT_DOWN,
	DOWN_MIDDLE,
	RIGHT_DOWN,
	RIGHT_MIDDLE,
	CENTER,
	RIGHT_UP_LOWER,
	UP_MIDDLE_LOWER,
	LEFT_UP_LOWER,
	LEFT_MIDDLE_LOWER,
	LEFT_DOWN_LOWER,
	DOWN_MIDDLE_LOWER,
	RIGHT_DOWN_LOWER,
	RIGHT_MIDDLE_LOWER,
	CENTER_LOWER,
	RESET,
};

enum Axe
{
	AXE_X,
	AXE_Y,
	AXE_Z,
	ALL_AXE,
	RESET_X,
	RESET_Y,
	RESET_Z,
	RESET_ALL,
};

enum SnapToAxe
{
	SNAP_X,
	SNAP_Y,
	SNAP_Z,
	SNAP_MX,
	SNAP_MY,
	SNAP_MZ,
};

class FPluginForLevelDesignEdModeToolkit : public FModeToolkit
{
public:

	FPluginForLevelDesignEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

	UPROPERTY(EditAnywhere)
	float Float_Scale = 0;

	UFUNCTION(BlueprintCallable)
	void ChangeValueScale(float _Value);	
	
	UPROPERTY(EditAnywhere)
	float Float_MinRot = 0;

	UFUNCTION(BlueprintCallable)
		void ChangeValueMinRot(float _Value);	
	
	UPROPERTY(EditAnywhere)
		float Float_MaxRot = 360;

	UFUNCTION(BlueprintCallable)
		void ChangeValueMaxRot(float _Value);

	 
protected:

private:

	TSharedPtr<SWidget> ToolkitWidget;
};

