#include "StateManagementEditor.h"

DEFINE_LOG_CATEGORY(StateManagementEditor);

#define LOCTEXT_NAMESPACE "FStateManagementEditor"

void FStateManagementEditor::StartupModule()
{
	UE_LOG(StateManagementEditor, Warning, TEXT("StateManagementEditor module has been loaded"));
}

void FStateManagementEditor::ShutdownModule()
{
	UE_LOG(StateManagementEditor, Warning, TEXT("StateManagementEditor module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStateManagementEditor, StateManagementEditor)