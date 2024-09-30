// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AttemptStateTransitionProxy.h"

#include "Components/GameStateManagerComponent.h"
#include "Objects/StateObject.h"

UAttemptStateTransitionProxy::UAttemptStateTransitionProxy()
{
}

UAttemptStateTransitionProxy* UAttemptStateTransitionProxy::CreateProxyObjectForAttemptStateTransition(
	UGameStateManagerComponent* GameStateManagerComponent, TSubclassOf<UStateObject> StateClass)
{
	UAttemptStateTransitionProxy* Proxy = NewObject<UAttemptStateTransitionProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->AttemptStateTransition(GameStateManagerComponent, StateClass);
	return Proxy;
}

void UAttemptStateTransitionProxy::BeginDestroy()
{
	UObject::BeginDestroy();
}


bool UAttemptStateTransitionProxy::AttemptStateTransition(UGameStateManagerComponent* GameStateManagerComponent,
                                                          TSubclassOf<UStateObject> StateClass)
{
	if(GameStateManagerComponent)
	{
		m_GameStateManagerComponent = GameStateManagerComponent;
		UE_LOG(LogTemp, Warning, TEXT("AttemptStateTransitionProxy::AttemptStateTransition - Attempting to transition to State [%s]"), *StateClass->GetName());
		
		if(GameStateManagerComponent->AttemptStateTransition(StateClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("AttemptStateTransitionProxy::AttemptStateTransition - Transition to State [%s] successful"), *StateClass->GetName());
			GameStateManagerComponent->GetActiveStateObject()->OnStateExited.AddUniqueDynamic(this, &UAttemptStateTransitionProxy::OnStateExitedReceived);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("AttemptStateTransitionProxy::AttemptStateTransition - Failed to transition to State [%s]"), *StateClass->GetName());
	return false;
}

void UAttemptStateTransitionProxy::OnStateExitedReceived()
{
	UnbindDelegates();
	OnStateExited.Broadcast();
}

void UAttemptStateTransitionProxy::UnbindDelegates()
{
	m_GameStateManagerComponent->GetActiveStateObject()->OnStateExited.RemoveDynamic(this, &UAttemptStateTransitionProxy::OnStateExitedReceived);
}
