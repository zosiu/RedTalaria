// Copyright (c) CD PROJEKT S.A. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UEdGraph;
class UEdGraphNode;
class UObject;

class FRedHermesGraphNodeEndpointUtils
{
public:
	static UObject* GetGraphOwnerAsset(const UEdGraph* Graph);

	static FString GetFocusUrlForGuidInGraph(const FGuid& Guid, const UEdGraph* Graph);
	static FString GetFocusUrlForNodeInGraph(const UEdGraphNode* Node, const UEdGraph* Graph);
	static FString GetOpenJumpTargetUrlForGuidAndNodeInGraph(const FGuid& Guid, const UEdGraphNode* Node, const UEdGraph* Graph);
	static FString GetOpenJumpTargetUrlForNodeInGraph(const UEdGraphNode* Node, const UEdGraph* Graph);
};
