// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MasterStateObject.h"

void UMasterStateObject::StateEntered_Implementation()
{
	Super::StateEntered_Implementation();
	AttemptStateTransition(m_InitialStateClass);
}

bool UMasterStateObject::AttemptStateTransition_Implementation(TSubclassOf<UStateObject> StateClass)
{
	if(!IsValid(StateClass))
	{
		UE_LOG(LogTemp, Error, TEXT("MasterStateObject: Cannot Transition to Invalid State Class"));
		return false;
	}
	
	UStateObject* StateObject = GetStateObject(StateClass);
	checkf(IsValid(StateObject), TEXT("MasterStateObject: State Object with class [%s] is Invalid"), *GetNameSafe(StateClass));
	
	// After this point, it is safe to assume that the State Object is completely valid and has been added to the StateMap
	if(IsValid(m_ActiveInternalState))
	{
		m_ActiveInternalState->ExitState(false);
		m_ActiveInternalState->PostStateExited.RemoveDynamic(this, &UMasterStateObject::OnStateExited);
	}
	const bool& bSuccess = StateObject->EnterState(false);
	if(bSuccess)
	{
		OnStateTransitioned(m_ActiveInternalState, StateObject);
		m_ActiveInternalState = StateObject;
		OnStateTransition.Broadcast(StateObject);
		m_ActiveInternalState->PostStateExited.AddDynamic(this, &UMasterStateObject::OnStateExited);
	}
	return bSuccess;
}

UStateObject* UMasterStateObject::GetStateObject(TSubclassOf<UStateObject> StateClass, bool bCreateIfNotPresent)
{
	UE_LOG(LogTemp, Display, TEXT("MasterStateObject: Attempting to Retrieve State Object [%s]"), *GetNameSafe(StateClass));
	if(bCreateIfNotPresent)
	{
		UE_LOG(LogTemp, Display, TEXT("MasterStateObject: Creating [%s] if not Present"), *GetNameSafe(StateClass));
		if(m_InternalStateMap.Contains(StateClass))
		{
			UE_LOG(LogTemp, Display, TEXT("State Map Contains State [%s]"), *GetNameSafe(StateClass));
			return m_InternalStateMap.FindRef(StateClass);
		}
		AddState(StateClass);
	}
	UE_LOG(LogTemp, Display, TEXT("MasterStateObject: Retrieving State Class [%s]"), *GetNameSafe(StateClass));
	return m_InternalStateMap.FindRef(StateClass);
}

UStateObject* UMasterStateObject::GetStateObjectUnsafe(TSubclassOf<UStateObject> StateClass) const
{
	return m_InternalStateMap.FindRef(StateClass);
}

bool UMasterStateObject::AddState(const TSubclassOf<UStateObject> StateClass)
{
	if(IsValid(StateClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("MasterStateObject: StateMap does not contain [%s]. Creating new instance"), *GetNameSafe(StateClass));
		UStateObject* StateObject = NewObject<UStateObject>(this, StateClass);

		if(!IsValid(StateObject))
		{
			UE_LOG(LogTemp, Error, TEXT("MasterStateObject: Creation of [%s] failed"), *GetNameSafe(StateClass));
			return false;
		}
		m_InternalStateMap.Add(StateClass, StateObject);
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("MasterStateObject: State Class is Invalid, Cannot Add to Manager"));
	return false;
}

void UMasterStateObject::OnStateTransitioned_Implementation(UStateObject* OldState, UStateObject* NewState)
{
	UE_LOG(LogTemp, Display, TEXT("MasterStateObject: Transitioned from [%s] to [%s]"), *GetNameSafe(OldState), *GetNameSafe(NewState));
}

void UMasterStateObject::OnStateExited(UStateObject* StateObject)
{
	UE_LOG(LogTemp, Display, TEXT("MasterStateObject: State [%s] Exited"), *GetNameSafe(StateObject));
	OnStateTransitioned(m_ActiveInternalState, nullptr);
}
