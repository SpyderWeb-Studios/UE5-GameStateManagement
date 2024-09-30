// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/StateObject.h"
#include "MasterStateObject.generated.h"

/**
 * This state acts as a way to nest states within a state. This is useful for creating complex state machines, 
  and for creating states that have their own internal states.
 */
UCLASS()
class GAMESTATEMANAGEMENT_API UMasterStateObject : public UStateObject
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInternalStateTransition, UStateObject*, NewState);
	
public:
	
	virtual void StateEntered_Implementation() override;
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable, Category="Game State Management")
	bool AttemptStateTransition(TSubclassOf<UStateObject> StateClass);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, meta=(DeterminesOutputType="StateClass"))
	UStateObject* GetStateObject(TSubclassOf<UStateObject> StateClass, bool bCreateIfNotPresent = true); 

	UFUNCTION(BlueprintAuthorityOnly, BlueprintPure, meta=(DeterminesOutputType="StateClass"))
	UStateObject* GetStateObjectUnsafe(TSubclassOf<UStateObject> StateClass) const;
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintPure)
	UStateObject* GetActiveStateObject() const { return m_ActiveInternalState; }

	UFUNCTION(BlueprintAuthorityOnly, BlueprintPure, meta=(DeterminesOutputType="StateClass"))
	UStateObject* GetActiveStateObjectAs(TSubclassOf<UStateObject> StateClass) const { return m_ActiveInternalState; }



	UPROPERTY(BlueprintAssignable, DisplayName="On State Transition")
	FOnInternalStateTransition OnStateTransition;
	
protected:

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Game State Management")
	bool AddState(const TSubclassOf<UStateObject> StateClass);

	UPROPERTY(BlueprintReadOnly, DisplayName="State Map")
	TMap<TSubclassOf<UStateObject>, TObjectPtr<UStateObject>> m_InternalStateMap;

	TObjectPtr<UStateObject> m_ActiveInternalState = nullptr;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateTransitioned(UStateObject* OldState, UStateObject* NewState);

	/**
	 * @brief The initial state class to use when this state is entered.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Initial State Class")
	TSubclassOf<UStateObject> m_InitialStateClass = nullptr;
private:
	
	UFUNCTION()
	void OnStateExited(UStateObject* StateObject);
};
