// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginForLevelDesignEdModeToolkit.h"
#include "PluginForLevelDesignEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "EditorModeManager.h"
#include <Engine/Engine.h>
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "PropertyHandle.h"
#include "Components/SlateWrapperTypes.h"



#define LOCTEXT_NAMESPACE "FPluginForLevelDesignEdModeToolkit"




FPluginForLevelDesignEdModeToolkit::FPluginForLevelDesignEdModeToolkit()
{

}

void FPluginForLevelDesignEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(PivotType pivot)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("ChangePivotTransactionName", "Change Pivot"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					FVector TempMin;
					FVector TempMax;

					FVector TPos;
					// Register actor in opened transaction (undo/redo)
					// Move actor to given location
					//LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0)); //Y,Z,X
					//LevelActor->SetPivotOffset(LevelActor->GetPivotOffset() + InOffset);

					//UMeshComponent* movC = LevelActor->FindComponentByClass<UMeshComponent>();
					//UStaticMeshComponent newMesh = UStaticMeshComponent();

					UStaticMeshComponent* mesh = LevelActor->FindComponentByClass<UStaticMeshComponent>();
					FVector tempPivot = FVector::ZeroVector;
					if (mesh != nullptr)
					{
						mesh->GetLocalBounds(TempMin, TempMax);
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "No static mesh component found");
						GEditor->EndTransaction();
						return FReply::Handled();
					}
					switch (pivot)
					{
					case RIGHT_UP_UPPER:
						tempPivot = (TempMin - TempMax) / 2.f;
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = TempMax.Z;
						break;
					case UP_MIDDLE_UPPER:
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = TempMax.Z;
						break;
					case LEFT_UP_UPPER:
						tempPivot = TempMax;
						break;
					case LEFT_MIDDLE_UPPER:
						tempPivot.Y = (TempMax.Y - TempMin.Y) / 2.f;
						tempPivot.Z = TempMax.Z;
						break;
					case LEFT_DOWN_UPPER:
						tempPivot.X = TempMin.X;
						tempPivot.Y = TempMax.Y;
						tempPivot.Z = TempMax.Z;
						break;
					case DOWN_MIDDLE_UPPER:
						tempPivot.X = TempMin.X;
						tempPivot.Z = TempMax.Z;
						break;
					case RIGHT_DOWN_UPPER:
						tempPivot = TempMin;
						tempPivot.Z = TempMax.Z;
						break;
					case RIGHT_MIDDLE_UPPER:
						tempPivot = (TempMin - TempMax) / 2.f;
						tempPivot.X = TempMin.X + TempMax.X;
						tempPivot.Z = TempMax.Z;
						break;
					case CENTER_UPPER:
						tempPivot.X = (TempMin.X + TempMax.X);
						tempPivot.Y = (TempMin.Y + TempMax.Y);
						tempPivot.Z = TempMax.Z;
						break;
					case RIGHT_UP:
						tempPivot = (TempMin - TempMax) / 2.f;
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case UP_MIDDLE:
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case LEFT_UP:
						tempPivot = TempMax;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case LEFT_MIDDLE:
						tempPivot.Y = (TempMax.Y - TempMin.Y) / 2.f;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case LEFT_DOWN:
						tempPivot.X = TempMin.X;
						tempPivot.Y = TempMax.Y;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case DOWN_MIDDLE:
						tempPivot.X = TempMin.X;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case RIGHT_DOWN:
						tempPivot = TempMin;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case RIGHT_MIDDLE:
						tempPivot = (TempMin - TempMax) / 2.f;
						tempPivot.X = TempMin.X + TempMax.X;
						tempPivot.Z = (TempMin.Z + TempMax.Z) / 2.f;
						break;
					case CENTER:
						tempPivot = ((TempMin + TempMax) / 2.f);
						break;
					case RIGHT_UP_LOWER:
						tempPivot = (TempMin - TempMax) / (float)2;
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = TempMin.Z;
						break;
					case UP_MIDDLE_LOWER:
						tempPivot.X = (TempMax.X - TempMin.X) / 2.f;
						tempPivot.Z = TempMin.Z;
						break;
					case LEFT_UP_LOWER:
						tempPivot = TempMax;
						tempPivot.Z = TempMin.Z;
						break;
					case LEFT_MIDDLE_LOWER:
						tempPivot.Y = (TempMax.Y - TempMin.Y) / 2.f;
						tempPivot.Z = TempMin.Z;
						break;
					case LEFT_DOWN_LOWER:
						tempPivot.X = TempMin.X;
						tempPivot.Y = TempMax.Y;
						tempPivot.Z = TempMin.Z;
						break;
					case DOWN_MIDDLE_LOWER:
						tempPivot.X = TempMin.X;
						tempPivot.Z = TempMin.Z;
						break;
					case RIGHT_DOWN_LOWER:
						tempPivot = TempMin;
						break;
					case RIGHT_MIDDLE_LOWER:
						tempPivot = (TempMin - TempMax) / 2.f;
						tempPivot.X = TempMin.X + TempMax.X;
						tempPivot.Z = TempMin.Z;
						break;
					case CENTER_LOWER:
						tempPivot = ((TempMin + TempMax));
						break;
					case RESET:
						tempPivot = FVector::ZeroVector;
						break;
					default:
						break;
					}

					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, tempPivot.ToString());

					LevelActor->Modify();
					LevelActor->SetPivotOffset((FVector((int)tempPivot.X, (int)tempPivot.Y, (int)tempPivot.Z)));
					GEditor->SetPivot(FVector((int)tempPivot.X, (int)tempPivot.Y, (int)tempPivot.Z), false, false);

					//FVector temp;

				   //LevelActor->GetActorTransform().GetLocation() = tempPivot;
				   //LevelActor->GetTransform().TransformPosition(temp) = tempPivot;

				   //temp = LevelActor->GetActorTransform().GetLocation();
				   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, LevelActor->GetPivotOffset().ToString());
				   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TPos.ToString());
				   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TempMax.ToString());
				   //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,temp.ToString());

				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const PivotType pivot)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, pivot);
		}
		static FReply OnButtonClickRandom(Axe _axe, FPluginForLevelDesignEdModeToolkit* plugin)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("ChangeRotationTransactionName", "Change Rotation"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					LevelActor->Modify();
					FVector temp = LevelActor->GetActorRotation().Euler();
					//LevelActor->offset
					//Y,Z,X
					switch (_axe)
					{
					case AXE_X:
						LevelActor->SetActorRotation(FRotator(LevelActor->GetActorRotation().Euler().Y, LevelActor->GetActorRotation().Euler().Z, rand() %(int) (plugin->Float_MaxRot- plugin->Float_MinRot) + plugin->Float_MinRot));
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorRightVector(),180-rand()%360));
						break;
					case AXE_Y:
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorForwardVector(),180-rand()%360));
						LevelActor->SetActorRotation(FRotator(rand() % (int)(plugin->Float_MaxRot - plugin->Float_MinRot) + plugin->Float_MinRot, LevelActor->GetActorRotation().Euler().Z, LevelActor->GetActorRotation().Euler().X));
						break;
					case AXE_Z:
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorForwardVector(),180-rand()%360));
						LevelActor->SetActorRotation(FRotator(LevelActor->GetActorRotation().Euler().Y, rand() % (int)(plugin->Float_MaxRot - plugin->Float_MinRot) + plugin->Float_MinRot, LevelActor->GetActorRotation().Euler().X));
						break;
					case ALL_AXE:
						LevelActor->SetActorRotation(FRotator(rand() % 360, rand() % 360, rand() % (int)(plugin->Float_MaxRot - plugin->Float_MinRot) + plugin->Float_MinRot));
						break;
					case RESET_X:
						LevelActor->SetActorRotation(FRotator(LevelActor->GetActorRotation().Euler().Y, LevelActor->GetActorRotation().Euler().Z, 0));
						break;
					case RESET_Y:
						LevelActor->SetActorRotation(FRotator(0, LevelActor->GetActorRotation().Euler().Z, LevelActor->GetActorRotation().Euler().X));
						break;
					case RESET_Z:
						LevelActor->SetActorRotation(FRotator(LevelActor->GetActorRotation().Euler().Y, 0, LevelActor->GetActorRotation().Euler().X));
						break;
					case RESET_ALL:
						LevelActor->SetActorRotation(FRotator(0, 0, 0));
						break;
					default:
						break;
					}
				}
			}
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static FReply OnButtonClickSnap(SnapToAxe snap)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("SnapToActor", "Snap Actors"));
			int32 nbActor = 0;
			AActor* firstActor = nullptr;
			AActor* secondActor = nullptr;
			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (nbActor == 0 && Cast<AActor>(*Iter))
				{
					firstActor = Cast<AActor>(*Iter);
					nbActor++;
				}
				else if (nbActor == 1 && Cast<AActor>(*Iter))
				{
					secondActor = Cast<AActor>(*Iter);
					nbActor++;
				}
				else if (nbActor >= 2 && Cast<AActor>(*Iter))
				{
					nbActor++;
				}
			}

			if (nbActor != 2)
			{
				if (nbActor < 2)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Only one actor selectioned");
				}
				else if (nbActor > 2)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "More than two actors selectioned");
				}
				GEditor->EndTransaction();
				return FReply::Handled();
			}
			else
			{
				if (firstActor != nullptr && secondActor != nullptr)
				{

					FVector tempFirstPivot = firstActor->GetPivotOffset();
					FVector tempSecondPivot = secondActor->GetPivotOffset();
					FVector fTempMin;
					FVector fTempMax;
					FVector sTempMin;
					FVector sTempMax;
					FVector ftempPivot = FVector::ZeroVector;
					FVector stempPivot = FVector::ZeroVector;
					UStaticMeshComponent* fmesh = firstActor->FindComponentByClass<UStaticMeshComponent>();
					UStaticMeshComponent* smesh = secondActor->FindComponentByClass<UStaticMeshComponent>();

					if (fmesh != nullptr && smesh != nullptr)
					{
						fmesh->GetLocalBounds(fTempMin, fTempMax);
						smesh->GetLocalBounds(sTempMin, sTempMax);

						firstActor->Modify();
						secondActor->Modify();

						firstActor->SetPivotOffset((FVector((int)ftempPivot.X, (int)ftempPivot.Y, (int)ftempPivot.Z)));
						secondActor->SetPivotOffset((FVector((int)stempPivot.X, (int)stempPivot.Y, (int)stempPivot.Z)));

						switch (snap)
						{
						case SNAP_X:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector((int)(fTempMax.X + sTempMax.X), 0, 0));
							break;
						case SNAP_Y:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector(0, (int)(fTempMax.Y + sTempMax.Y), 0));
							break;
						case SNAP_Z:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector(0, 0, (int)(fTempMax.Z + sTempMax.Z)));
							break;
						case SNAP_MX:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector((int)(fTempMin.X + sTempMin.X), 0, 0));
							break;
						case SNAP_MY:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector(0, (int)(fTempMin.Y + sTempMin.Y), 0));
							break;
						case SNAP_MZ:

							secondActor->SetActorLocation(firstActor->GetActorLocation() + FVector(0, 0, (int)(fTempMin.Z + sTempMin.Z)));
							break;
						default:
							break;
						}
						firstActor->SetPivotOffset(tempFirstPivot);
						secondActor->SetPivotOffset(tempSecondPivot);
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "No static mesh component found");
						GEditor->EndTransaction();
						return FReply::Handled();
					}


				}
			}

			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static FReply OnButtonClickScale(Axe _scale, FPluginForLevelDesignEdModeToolkit* plugin)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("ChangeRotationTransactionName", "Change Rotation"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					LevelActor->Modify();
					FVector temp = LevelActor->GetActorRotation().Euler();
					//LevelActor->offset
					//Y,Z,X
					float tempRand = (rand() / (float)RAND_MAX) * 10.f;
					switch (_scale)
					{
					case AXE_X:
						LevelActor->SetActorScale3D(FVector(plugin->Float_Scale, LevelActor->GetActorScale3D().Y, LevelActor->GetActorScale3D().Z));
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorRightVector(),180-rand()%360));
						break;
					case AXE_Y:
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorForwardVector(),180-rand()%360));
						LevelActor->SetActorScale3D(FVector(LevelActor->GetActorScale3D().X, plugin->Float_Scale, LevelActor->GetActorScale3D().Z));
						break;
					case AXE_Z:
						//LevelActor->SetActorRotation(FQuat(LevelActor->GetActorForwardVector(),180-rand()%360));
						LevelActor->SetActorScale3D(FVector(LevelActor->GetActorScale3D().X, LevelActor->GetActorScale3D().Y, plugin->Float_Scale));
						break;
					case ALL_AXE:
						LevelActor->SetActorScale3D(FVector(plugin->Float_Scale, plugin->Float_Scale, plugin->Float_Scale));
						break;
					case RESET_X:
						LevelActor->SetActorScale3D(FVector(1, LevelActor->GetActorScale3D().Y, LevelActor->GetActorScale3D().Z));
						break;
					case RESET_Y:
						LevelActor->SetActorScale3D(FVector(LevelActor->GetActorScale3D().X, 1, LevelActor->GetActorScale3D().Z));
						break;
					case RESET_Z:
						LevelActor->SetActorScale3D(FVector(LevelActor->GetActorScale3D().X, LevelActor->GetActorScale3D().Y, 1));
						break;
					case RESET_ALL:
						LevelActor->SetActorScale3D(FVector::OneVector);
						break;
					default:
						break;
					}
				}
			}
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButtonRandom(FText InLabel, const Axe axe, FPluginForLevelDesignEdModeToolkit* plugin)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClickRandom, axe,plugin);
		}

		static TSharedRef<SWidget> MakeButtonSnapping(FText InLabel, const SnapToAxe snap)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClickSnap, snap);
		}

		static TSharedRef<SWidget> MakeButtonScale(FText InLabel, const Axe scale, FPluginForLevelDesignEdModeToolkit* plugin)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClickScale, scale, plugin);
		}
	};

	//const float Factor = 256.0f;


	SAssignNew(ToolkitWidget, SBorder)

		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightUpUpper", "RightUpUpper"), PivotType::RIGHT_UP_UPPER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("UpMiddleUpper", "UpMiddleUpper"), PivotType::UP_MIDDLE_UPPER)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftUpUpper", "LeftUpUpper"), PivotType::LEFT_UP_UPPER)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightMiddleUpper", "RightMiddleUpper"), PivotType::RIGHT_MIDDLE_UPPER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("CenterUpper", "CenterUpper"), PivotType::CENTER_UPPER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftMiddleUpper", "LeftMiddleUpper"), PivotType::LEFT_MIDDLE_UPPER)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightDownUpper", "RightDownUpper"), PivotType::RIGHT_DOWN_UPPER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("DownMiddleUpper", "DownMiddleUpper"), PivotType::DOWN_MIDDLE_UPPER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftDownUpper", "LeftDownUpper"), PivotType::LEFT_DOWN_UPPER)
		]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(2)
		+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightUp", "RightUp"), PivotType::RIGHT_UP)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("UpMiddle", "UpMiddle"), PivotType::UP_MIDDLE)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftUp", "LeftUp"), PivotType::LEFT_UP)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightMiddle", "RightMiddle"), PivotType::RIGHT_MIDDLE)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("Center", "Center"), PivotType::CENTER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftMiddle", "LeftMiddle"), PivotType::LEFT_MIDDLE)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightDown", "RightDown"), PivotType::RIGHT_DOWN)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("DownMiddle", "DownMiddle"), PivotType::DOWN_MIDDLE)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftDown", "LeftDown"), PivotType::LEFT_DOWN)
		]

		]
	+ SVerticalBox::Slot()
		.MaxHeight(2)
		+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightUpLower", "RightUpLower"), PivotType::RIGHT_UP_LOWER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("UpMiddleLower", "UpMiddleLower"), PivotType::UP_MIDDLE_LOWER)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftUpLower", "LeftUpLower"), PivotType::LEFT_UP_LOWER)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightMiddleLower", "RightMiddleLower"), PivotType::RIGHT_MIDDLE_LOWER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("CenterLower", "CenterLower"), PivotType::CENTER_LOWER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftMiddleLower", "LeftMiddleLower"), PivotType::LEFT_MIDDLE_LOWER)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("RightDownLower", "RightDownLower"), PivotType::RIGHT_DOWN_LOWER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("DownMiddleLower", "DownMiddleLower"), PivotType::DOWN_MIDDLE_LOWER)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButton(LOCTEXT("LeftDownLower", "LeftDownLower"), PivotType::LEFT_DOWN_LOWER)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			Locals::MakeButton(LOCTEXT("Reset", "Reset"), PivotType::RESET)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
		.Text(FText::FromString(TEXT("Choose the minimum and the maximum of the rotation")))
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
		.Text(FText::FromString(TEXT("Minimum Rotation")))
		] 
	+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
		.Text(FText::FromString(TEXT("Maximum Rotation")))
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SSpinBox<float>)
			.MinValue(0)
		.MaxValue(360)
		.MinSliderValue(0)
		.MaxSliderValue(360)
		.Value(0)
		.OnValueChanged_Lambda([=](float NewValue)
			{
				//FString TheFloatStr = FString::SanitizeFloat(NewValue, 2);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TheFloatStr);
				ChangeValueMinRot(NewValue);
			})
		]
			+ SHorizontalBox::Slot()
		[
			SNew(SSpinBox<float>)
			.MinValue(0)
		.MaxValue(360)
		.MinSliderValue(0)
		.MaxSliderValue(360)
		.Value(360)
		.OnValueChanged_Lambda([=](float NewValue)
			{
				//FString TheFloatStr = FString::SanitizeFloat(NewValue, 2);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TheFloatStr);
				ChangeValueMaxRot(NewValue);
			})
		]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(2)
		+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Rand X", "Rand X"), AXE_X,this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Rand Y", "Rand Y"), AXE_Y,this)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Rand Z", "Rand Z"), AXE_Z,this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Rand All", "Rand All"), ALL_AXE,this)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Reset X", "Reset X"), RESET_X,this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Reset Y", "Reset Y"), RESET_Y,this)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Reset Z", "Reset Z"), RESET_Z,this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonRandom(LOCTEXT("Reset All", "Reset All"), RESET_ALL,this)
		]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(2)
		+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap X", "Snap X"), SNAP_X)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap Y", "Snap Y"), SNAP_Y)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap Z", "Snap Z"), SNAP_Z)
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap -X", "Snap -X"), SNAP_MX)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap -Y", "Snap -Y"), SNAP_MY)
		]

	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonSnapping(LOCTEXT("Snap -Z", "Snap -Z"), SNAP_MZ)
		]

		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
		.Text(FText::FromString(TEXT("Use this to change the value for the scale, between 0 and 100")))
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SSpinBox<float>)
			.MinValue(0)
		.MaxValue(100)
		.MinSliderValue(0)
		.MaxSliderValue(100)
		.Value(0)
		.OnValueChanged_Lambda([=](float NewValue)
			{
				//FString TheFloatStr = FString::SanitizeFloat(NewValue, 2);
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TheFloatStr);
				ChangeValueScale(NewValue);
			})
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()

		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Scale X", "Scale X"), AXE_X, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Scale Y", "Scale Y"), AXE_Y, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Scale Z", "Scale Z"), AXE_Z, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Scale All", "Scale All"), ALL_AXE, this)
		]

		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Reset X", "Reset X"), RESET_X, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Reset Y", "Reset Y"), RESET_Y, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Reset Z", "Reset Z"), RESET_Z, this)
		]
	+ SHorizontalBox::Slot()
		[
			Locals::MakeButtonScale(LOCTEXT("Reset All", "Reset All"), RESET_ALL, this)
		]

		]

		];


	FModeToolkit::Init(InitToolkitHost);
}

FName FPluginForLevelDesignEdModeToolkit::GetToolkitFName() const
{
	return FName("Level Design Tool");
}

FText FPluginForLevelDesignEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("PluginForLevelDesignEdModeToolkit", "DisplayName", "LevelDesign Tool");
}

class FEdMode* FPluginForLevelDesignEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FPluginForLevelDesignEdMode::EM_PluginForLevelDesignEdModeId);
}

void FPluginForLevelDesignEdModeToolkit::ChangeValueScale(float _Value)
{
	Float_Scale = _Value;
}

void FPluginForLevelDesignEdModeToolkit::ChangeValueMinRot(float _Value)
{
	Float_MinRot = _Value;
}

void FPluginForLevelDesignEdModeToolkit::ChangeValueMaxRot(float _Value)
{
	Float_MaxRot = _Value;

}

#undef LOCTEXT_NAMESPACE
