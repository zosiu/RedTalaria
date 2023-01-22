// Copyright (c) CD PROJEKT S.A. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EditorSubsystem.h"

#include "RedHermesGraphNodeEndpointEditorExtension.generated.h"

class FMenuBuilder;
class FExtender;
class FUICommandList;
class UEdGraph;
class UEdGraphNode;
class UEdGraphPin;

struct FMenuExtensionHookWithGuid
{
	FName MenuExtensionHook;
	FGuid Guid;

	friend bool operator==(const FMenuExtensionHookWithGuid& Lhs, const FMenuExtensionHookWithGuid& Rhs)
	{
		return Lhs.MenuExtensionHook == Rhs.MenuExtensionHook;
	}

	friend uint32 GetTypeHash(const FMenuExtensionHookWithGuid& Key)
	{
		return GetTypeHash(Key.MenuExtensionHook);
	}
};

UCLASS()
class REDHERMESGRAPHNODEENDPOINT_API URedHermesGraphNodeEndpointEditorExtension : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnNodeMenuExtensionHookRequestDelegate, const UEdGraphNode*, const UEdGraph*, TSet<FName>&);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnNodeMenuExtensionHookWithGuidRequestDelegate, const UEdGraphNode*, const UEdGraph*, TSet<FMenuExtensionHookWithGuid>&);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FOnNodeMenuExtensionHookRequestDelegate& OnNodeMenuExtensionHookRequest() { return OnNodeMenuExtensionHookRequestDelegate; }
	FOnNodeMenuExtensionHookWithGuidRequestDelegate& OnNodeMenuExtensionHookWithGuidRequest() { return OnNodeMenuExtensionHookWithGuidRequestDelegate; }

private:
	FOnNodeMenuExtensionHookRequestDelegate OnNodeMenuExtensionHookRequestDelegate;
	FOnNodeMenuExtensionHookWithGuidRequestDelegate OnNodeMenuExtensionHookWithGuidRequestDelegate;

	TSharedRef<FExtender> HandleOnExtendGraphEditorContextMenu(
		const TSharedRef<FUICommandList> CommandList,
		const UEdGraph* Graph,
		const UEdGraphNode* Node,
		const UEdGraphPin* Pin,
		bool bIsConst);

	struct FCopyUrlMenuEntry
	{
		FText Label;
		FText Tooltip;
		FString UrlToCopy;
	};

	static void HandleOnExtendContextMenu(FMenuBuilder& MenuBuilder, TArray<FCopyUrlMenuEntry> MenuEntries);
};
