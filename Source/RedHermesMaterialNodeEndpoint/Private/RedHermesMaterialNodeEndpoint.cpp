// Copyright (c) CD PROJEKT S.A. All Rights Reserved.

#include "RedHermesMaterialNodeEndpoint.h"

#include "Editor.h"
#include "RedHermesGraphNodeEndpoint.h"
#include "RedHermesGraphNodeEndpointEditorExtension.h"
#include "EdGraph/EdGraph.h"
#include "MaterialEditor/Private/MaterialEditor.h"
#include "MaterialGraph/MaterialGraphNode.h"
#include "Materials/Material.h"
#include "Modules/ModuleManager.h"
#include "Subsystems/AssetEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "FRedHermesMaterialNodeEndpointModule"

void FRedHermesMaterialNodeEndpointModule::StartupModule()
{
	if (const auto RedHermesGraphNodeEndpoint = GEditor->GetEditorSubsystem<URedHermesGraphNodeEndpoint>())
	{
		OnRequestDelegateHandle = RedHermesGraphNodeEndpoint->OnRequest().AddStatic(&FRedHermesMaterialNodeEndpointModule::HandleRequest);
	}

	if (const auto NodeExtensionSubsystem = GEditor->GetEditorSubsystem<URedHermesGraphNodeEndpointEditorExtension>())
	{
		OnNodeMenuExtensionHookWithGuidRequestDelegateHandle = NodeExtensionSubsystem->OnNodeMenuExtensionHookWithGuidRequest().AddStatic(&FRedHermesMaterialNodeEndpointModule::ProvideNodeExtensionHookWithGuid);
	}
}

void FRedHermesMaterialNodeEndpointModule::ShutdownModule()
{
	if (OnRequestDelegateHandle.IsValid())
	{
		if (GEditor != nullptr)
		{
			if (const auto RedHermesGraphNodeEndpoint = GEditor->GetEditorSubsystem<URedHermesGraphNodeEndpoint>())
			{
				RedHermesGraphNodeEndpoint->OnRequest().Remove(OnRequestDelegateHandle);
			}
			OnRequestDelegateHandle.Reset();
		}
	}

	if (OnNodeMenuExtensionHookWithGuidRequestDelegateHandle.IsValid())
	{
		if (GEditor != nullptr)
		{
			if (const auto NodeExtensionSubsystem = GEditor->GetEditorSubsystem<URedHermesGraphNodeEndpointEditorExtension>())
			{
				NodeExtensionSubsystem->OnNodeMenuExtensionHookWithGuidRequest().Remove(OnNodeMenuExtensionHookWithGuidRequestDelegateHandle);
			}
			OnNodeMenuExtensionHookWithGuidRequestDelegateHandle.Reset();
		}
	}
}

UMaterialExpression* FRedHermesMaterialNodeEndpointModule::GetMaterialExpressionInAsset(const FGuid& MaterialExpressionGuid, UObject* Asset)
{
	if (auto Material = Cast<UMaterial>(Asset))
	{
		UMaterialGraph* MaterialGraph = Cast<UMaterial>(Asset)->MaterialGraph;

		if (MaterialGraph == nullptr)
		{
			UMaterialGraph* NewMatGraph = CastChecked<UMaterialGraph>(NewObject<UEdGraph>(Material, UMaterialGraph::StaticClass(), NAME_None, RF_Transient));
			NewMatGraph->Material = Material;
			NewMatGraph->RebuildGraph();
			MaterialGraph = NewMatGraph;
		}

		for (const UEdGraphNode* Node : MaterialGraph->Nodes)
		{
			if (const auto* MaterialNode = Cast<UMaterialGraphNode>(Node))
			{
				if (UMaterialExpression* MaterialExpression = MaterialNode->MaterialExpression)
				{
					if (MaterialExpression->MaterialExpressionGuid == MaterialExpressionGuid)
					{
						return MaterialExpression;
					}
				}
			}
		}
	}

	return nullptr;
}

void FRedHermesMaterialNodeEndpointModule::HandleRequest(FGuid MaterialExpressionGuid, UObject* Asset, bool bOpenJumpTarget)
{
	if (UAssetEditorSubsystem* AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
	{
		if (AssetEditor->OpenEditorForAsset(Asset))
		{
			FMaterialEditor* TargetEditor = static_cast<FMaterialEditor*>(AssetEditor->FindEditorForAsset(Asset, true));
			FMaterialExpressionCollection& Collection = TargetEditor->Material->GetEditorOnlyData()->ExpressionCollection;

			for (const TObjectPtr<UMaterialExpression>& EditorExpression : Collection.Expressions)
			{
				if (EditorExpression->MaterialExpressionGuid == MaterialExpressionGuid)
				{
					TargetEditor->JumpToExpression(EditorExpression);
					return;
				}
			}
		}
	}
}

void FRedHermesMaterialNodeEndpointModule::ProvideNodeExtensionHookWithGuid(const UEdGraphNode* Node, const UEdGraph* /* Graph */, TSet<FMenuExtensionHookWithGuid>& ExtensionHookWithNodeIds)
{
	// TODO: handle comment nodes too
	// TODO: what about jump targets?
	// TODO: what about material functions? (GetGraphOwnerAsset probably needs to be tweaked)
	if (const auto* MaterialNode = Cast<UMaterialGraphNode>(Node))
	{
		if (const UMaterialExpression* MaterialExpression = MaterialNode->MaterialExpression)
		{
			ExtensionHookWithNodeIds.Emplace({FName(TEXT("MaterialSchemaNodeActions")), MaterialExpression->MaterialExpressionGuid});
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRedHermesMaterialNodeEndpointModule, RedHermesMaterialNodeEndpoint)
