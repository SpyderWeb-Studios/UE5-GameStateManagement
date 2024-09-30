// Copyright Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEvent);


/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, meta = (ShowWorldContextPin))
class GAMESTATEMANAGEMENT_API UStateObject : public UObject
{
	GENERATED_BODY()

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionStateEvent, UStateObject*, StateObject);
	
public:
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	bool EnterState(bool bDeferEnter);
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void UpdateState();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void ExitState(bool bDeferExit);

	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PreStateEntered;
	
	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PostStateEntered;
	
	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PreStateUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PostStateUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PreStateExited;
	
	UPROPERTY(BlueprintAssignable)
	FTransitionStateEvent PostStateExited;


	UPROPERTY(BlueprintAssignable)
	FOnStateEvent OnStateEntered;

	UPROPERTY(BlueprintAssignable)
	FOnStateEvent OnStateUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnStateEvent OnStateExited;

	
protected:
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void StateEntered();
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void StateExited();
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintNativeEvent, BlueprintCallable)
	void StateUpdated(int Index = 0);
	
	UPROPERTY(BlueprintReadOnly)
	AGameStateBase* GameStateRef;
	
private:

	int m_InternalStateIndex = 0;
};
