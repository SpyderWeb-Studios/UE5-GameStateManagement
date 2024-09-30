// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateObject.h"
#include "UObject/NoExportTypes.h"
#include "AttemptStateTransitionProxy.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UAttemptStateTransitionProxy : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnStateEvent OnStateExited;

	UAttemptStateTransitionProxy();

	// Called to perform the query internally
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static GAMESTATEMANAGEMENT_API UAttemptStateTransitionProxy* CreateProxyObjectForAttemptStateTransition(
		class UGameStateManagerComponent *GameStateManagerComponent,
		TSubclassOf<UStateObject> StateClass);

	//~ Begin UObject Interface
	virtual GAMESTATEMANAGEMENT_API void BeginDestroy() override;
	//~ End UObject Interface

protected:

	
	GAMESTATEMANAGEMENT_API bool AttemptStateTransition(
	class UGameStateManagerComponent *GameStateManagerComponent,
	TSubclassOf<UStateObject> StateClass);
	UFUNCTION()
	GAMESTATEMANAGEMENT_API void OnStateExitedReceived();

	TWeakObjectPtr<class UGameStateManagerComponent> m_GameStateManagerComponent;
private:
	
	GAMESTATEMANAGEMENT_API void UnbindDelegates();

};