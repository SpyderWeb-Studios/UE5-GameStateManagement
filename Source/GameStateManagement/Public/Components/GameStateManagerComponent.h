// Copyright Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStateManagerComponent.generated.h"

class UStateObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESTATEMANAGEMENT_API UGameStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateTransition, UStateObject*, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStateTransitionEvent, UStateObject*, NewState, UStateObject*, CurrentState, TSubclassOf<UStateObject>, NewStateClass, TSubclassOf<UStateObject>, CurrentStateClass);
	

public:	
	// Sets default values for this component's properties
	UGameStateManagerComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable, Category="Game State Management")
	bool AttemptStateTransition(TSubclassOf<UStateObject> StateClass);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Game State Management")
	void PinActiveState(TSubclassOf<UStateObject> StateClass);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Game State Management")
	void UnpinActiveState(TSubclassOf<UStateObject> StateClass);
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, meta=(DeterminesOutputType="StateClass"))
	UStateObject* GetStateObject(TSubclassOf<UStateObject> StateClass, bool bCreateIfNotPresent = true); 

	UFUNCTION(BlueprintAuthorityOnly, BlueprintPure, meta=(DeterminesOutputType="StateClass"))
	UStateObject* GetStateObjectUnsafe(TSubclassOf<UStateObject> StateClass) const;
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintPure)
	UStateObject* GetActiveStateObject() const { return m_ActiveState; }

	UFUNCTION(BlueprintPure)
	bool IsStateActive(TSubclassOf<UStateObject> StateClass) const;
	
	UPROPERTY(BlueprintAssignable, DisplayName="On State Transition")
	FOnStateTransition OnStateTransition;

	UPROPERTY(BlueprintAssignable, DisplayName="On State Transition Event")
	FOnStateTransitionEvent OnStateTransitionEvent;
	
protected:

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Game State Management")
	bool AddStateToManager(const TSubclassOf<UStateObject> StateClass);

	UPROPERTY(BlueprintReadOnly, DisplayName="State Map")
	TMap<TSubclassOf<UStateObject>, TObjectPtr<UStateObject>> m_StateMap;

	TObjectPtr<UStateObject> m_ActiveState = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Initial State Class")
	TSubclassOf<UStateObject> m_InitialStateClass = nullptr;
	

	bool bActiveStatePinned = false;
	TSubclassOf<UStateObject> m_PinnedStateClass = nullptr;
	
};
