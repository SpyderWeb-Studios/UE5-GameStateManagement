// Copyright Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/GameStateManagerComponent.h"

#include <Windows.Graphics.Display.h>

#include "Objects/StateObject.h"

// Sets default values for this component's properties
UGameStateManagerComponent::UGameStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGameStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	AttemptStateTransition(m_InitialStateClass);
}

void UGameStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(IsValid(m_ActiveState))
	{
		m_ActiveState->TickState(DeltaTime);
	}
}

void UGameStateManagerComponent::PinActiveState(TSubclassOf<UStateObject> StateClass)
{
	// If the state class is specified, then we should check if the active state is the same as the specified state class
	// If the state class is not specified, then we should check if the active state is valid
	if(
		(IsValid(StateClass) && IsValid(m_ActiveState) && m_ActiveState->GetClass() == StateClass) ||
		(IsValid(m_ActiveState)))
	{
		UE_LOG(LogTemp, Display, TEXT("Game State Manager: Pinning Active State"));
		bActiveStatePinned = true;
	}
}

void UGameStateManagerComponent::UnpinActiveState(TSubclassOf<UStateObject> StateClass)
{
	if(bActiveStatePinned && IsValid(StateClass) && m_ActiveState->GetClass() == StateClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Game State Manager: Unpinning Active State, Class Specified: [%s]"), *GetNameSafe(StateClass));
		bActiveStatePinned = false;
	}
	if(!IsValid(StateClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Game State Manager: Unpinning Active State"));
		bActiveStatePinned = false;
	}
	if(IsValid(m_PinnedStateClass))
	{
		AttemptStateTransition(m_PinnedStateClass);
		m_PinnedStateClass = nullptr;
	}
}

UStateObject* UGameStateManagerComponent::GetStateObjectUnsafe(TSubclassOf<UStateObject> StateClass) const
{
	return m_StateMap.FindRef(StateClass);
}

bool UGameStateManagerComponent::IsStateActive(TSubclassOf<UStateObject> StateClass) const
{
	return IsValid(m_ActiveState) && m_ActiveState->GetClass() == StateClass;
}

bool UGameStateManagerComponent::AddStateToManager(const TSubclassOf<UStateObject> StateClass)
{
	if(IsValid(StateClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game State Manager: StateMap does not contain [%s]. Creating new instance"), *GetNameSafe(StateClass));
		UStateObject* StateObject = NewObject<UStateObject>(this, StateClass);

		if(!IsValid(StateObject))
		{
			UE_LOG(LogTemp, Error, TEXT("Game State Manager: Creation of [%s] failed"), *GetNameSafe(StateClass));
			return false;
		}
		m_StateMap.Add(StateClass, StateObject);
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("Game State Manager: State Class is Invalid, Cannot Add to Manager"));
	return false;
}

UStateObject* UGameStateManagerComponent::GetStateObject(TSubclassOf<UStateObject> StateClass, bool bCreateIfNotPresent)
{
	UE_LOG(LogTemp, Display, TEXT("Game State Manager: Attempting to Retrieve State Object [%s]"), *GetNameSafe(StateClass));
	if(bCreateIfNotPresent)
	{
		UE_LOG(LogTemp, Display, TEXT("Game State Manager: Creating [%s] if not Present"), *GetNameSafe(StateClass));
		if(m_StateMap.Contains(StateClass))
		{
			UE_LOG(LogTemp, Display, TEXT("State Map Contains State [%s]"), *GetNameSafe(StateClass));
			return m_StateMap.FindRef(StateClass);
		}
		AddStateToManager(StateClass);
	}
	UE_LOG(LogTemp, Display, TEXT("Game State Manager: Retrieving State Class [%s]"), *GetNameSafe(StateClass));
	return m_StateMap.FindRef(StateClass);
}

bool UGameStateManagerComponent::AttemptStateTransition_Implementation(TSubclassOf<UStateObject> StateClass)
{
	if(!IsValid(StateClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Game State Manager: Cannot Transition to Invalid State Class"));
		return false;
	}

	if(bActiveStatePinned)
	{
		UE_LOG(LogTemp, Display, TEXT("Game State Manager: Active State is Pinned, Cannot Transition"));
		m_PinnedStateClass = StateClass;
		m_ActiveState->OnStateExitAttempted.Broadcast();
		return false;
	}
	
	UStateObject* StateObject = GetStateObject(StateClass);
	checkf(IsValid(StateObject), TEXT("Game State Manager: State Object with class [%s] is Invalid"), *GetNameSafe(StateClass));
	
	// After this point, it is safe to assume that the State Object is completely valid and has been added to the StateMap
	if(IsValid(m_ActiveState))
	{
		m_ActiveState->ExitState(false);
	}
	const bool& bSuccess = StateObject->EnterState(false);
	if(bSuccess)
	{
		m_ActiveState = StateObject;
		OnStateTransition.Broadcast(StateObject);
		OnStateTransitionEvent.Broadcast(StateObject, m_ActiveState, StateClass, m_ActiveState->GetClass());
	}
	return bSuccess;
}
