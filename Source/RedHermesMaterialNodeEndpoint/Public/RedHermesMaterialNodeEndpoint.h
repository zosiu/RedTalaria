// Copyright (c) CD PROJEKT S.A. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleInterface.h"
#include "UObject/NameTypes.h"

struct FMenuExtensionHookWithGuid;
class UEdGraphNode;
class UEdGraph;
class UMaterialExpression;
class UObject;

class FRedHermesMaterialNodeEndpointModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FDelegateHandle OnRequestDelegateHandle;
	FDelegateHandle OnNodeMenuExtensionHookWithGuidRequestDelegateHandle;

	static UMaterialExpression* GetMaterialExpressionInAsset(const FGuid& MaterialExpressionGuid, UObject* Asset);
	static void HandleRequest(FGuid MaterialExpressionGuid, UObject* Asset, bool bOpenJumpTarget);
	static void ProvideNodeExtensionHookWithGuid(const UEdGraphNode* Node, const UEdGraph* Graph, TSet<FMenuExtensionHookWithGuid>& ExtensionHookWithNodeIds);
};
