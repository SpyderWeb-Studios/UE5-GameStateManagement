// Copyright Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/GameStateManagerComponent.h"

#include <Windows.Graphics.Display.h>

#include "Objects/StateObject.h"

// Sets default values for this component's properties
UGameStateManagerComponent::UGameStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGameStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	AttemptStateTransition(m_InitialStateClass);
}

UStateObject* UGameStateManagerComponent::GetStateObjectUnsafe(TSubclassOf<UStateObject> StateClass) const
{
	return m_StateMap.FindRef(StateClass);
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
	}
	return bSuccess;
}
