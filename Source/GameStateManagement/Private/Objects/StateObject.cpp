// Copyright Spyderweb Studios Ltd. All Rights Reserved.


#include "Objects/StateObject.h"
#include "Kismet/GameplayStatics.h"

bool UStateObject::EnterState_Implementation(bool bDeferEnter)
{
	UE_LOG(LogTemp, Display, TEXT("Entering State [%s]"), *GetNameSafe(this));
	PreStateEntered.Broadcast(this);
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
	PreStateEntered.Broadcast(this);
	StateUpdated();
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
}

void UStateObject::StateExited_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("State [%s] Exited"), *GetNameSafe(this));
	PostStateExited.Broadcast(this);
}

void UStateObject::StateUpdated_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("State [%s] Updated"), *GetNameSafe(this));
	PostStateUpdated.Broadcast(this);
}
