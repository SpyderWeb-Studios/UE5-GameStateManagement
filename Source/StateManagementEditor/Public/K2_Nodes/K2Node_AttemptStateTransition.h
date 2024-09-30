// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_AttemptStateTransition.generated.h"

/**
 * 
 */
UCLASS()
class STATEMANAGEMENTEDITOR_API UK2Node_AttemptStateTransition : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()

	UK2Node_AttemptStateTransition();
	
	//~ Begin UEdGraphNode Interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual FText GetMenuCategory() const override;
	//~ End UK2Node Interface
};
