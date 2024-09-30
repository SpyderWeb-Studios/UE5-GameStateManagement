// Fill out your copyright notice in the Description page of Project Settings.


#include "K2_Nodes/K2Node_AttemptStateTransition.h"
#include "Objects/AttemptStateTransitionProxy.h"

#define LOCTEXT_NAMESPACE "K2Node"

UK2Node_AttemptStateTransition::UK2Node_AttemptStateTransition()
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UAttemptStateTransitionProxy, CreateProxyObjectForAttemptStateTransition);
	ProxyFactoryClass = UAttemptStateTransitionProxy::StaticClass();
	ProxyClass = UAttemptStateTransitionProxy::StaticClass();
}

FText UK2Node_AttemptStateTransition::GetTooltipText() const
{
	return LOCTEXT("K2Node_StateTransition", "Attempts to make a State Transition given a Game State Management Component");
}

FText UK2Node_AttemptStateTransition::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("AttemptTransition", "Attempt State Transition");
}

void UK2Node_AttemptStateTransition::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	Super::GetPinHoverText(Pin, HoverTextOut);

	static const FName NAME_OnStateExited = FName(TEXT("On State Exited"));

	if (Pin.PinName == NAME_OnStateExited)
	{
		FText ToolTipText = LOCTEXT("K2Node_AttemptTransition_OnStateExited_Tooltip", "Event called when the State is exited");
		HoverTextOut = FString::Printf(TEXT("%s\n%s"), *ToolTipText.ToString(), *HoverTextOut);
	}
}

FText UK2Node_AttemptStateTransition::GetMenuCategory() const
{
	return LOCTEXT("AttemptTransition", "Game State Management");
}
