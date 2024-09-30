// Copyright Spyderweb Studios Ltd. All Rights Reserved.


#include "Objects/StateObject.h"
#include "Kismet/GameplayStatics.h"

bool UStateObject::EnterState_Implementation(bool bDeferEnter)
{
	UE_LOG(LogTemp, Display, TEXT("Entering State [%s]"), *GetNameSafe(this));
	PreStateEntered.Broadcast(this);
	
	// Reset the internal state index
	m_InternalStateIndex = 0;
	
	if(!bDeferEnter)
	{
		StateEntered();
	}
	GameStateRef = UGameplayStatics::GetGameState(this);
	return true;
}

void UStateObject::UpdateState_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Updating State [%s]"), *GetNameSafe(this));
	PreStateUpdated.Broadcast(this);
	m_InternalStateIndex++;
	StateUpdated(m_InternalStateIndex);
}

void UStateObject::ExitState_Implementation(bool bDeferExit)
{
UE_LOG(LogTemp, Display, TEXT("Exiting State [%s]"), *GetNameSafe(this));
	PreStateExited.Broadcast(this);
	if(!bDeferExit)
	{
		StateExited();
	}
}

void UStateObject::StateEntered_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("State [%s] Entered"), *GetNameSafe(this));
	PostStateEntered.Broadcast(this);
	OnStateEntered.Broadcast();
}

void UStateObject::StateExited_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("State [%s] Exited"), *GetNameSafe(this));
	PostStateExited.Broadcast(this);
	OnStateExited.Broadcast();
}

void UStateObject::StateUpdated_Implementation(int Index)
{
	UE_LOG(LogTemp, Display, TEXT("State [%s] Updated"), *GetNameSafe(this));
	PostStateUpdated.Broadcast(this);
	OnStateUpdated.Broadcast();
}

void UStateObject::TickState_Implementation(float DeltaTime)
{
	// UE_LOG(LogTemp, Display, TEXT("State [%s] Ticked"), *GetNameSafe(this));
}
