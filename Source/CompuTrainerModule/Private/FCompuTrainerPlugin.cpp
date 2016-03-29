#include "CompuTrainerPluginModulePCH.h"
#include "CompuTrainerModule.h"

#define LOCTEXT_NAMESPACE "FCompuTrainerModule"

void FCompuTrainerModuleModuleImpl::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("FCompuTrainerModule::StartupModule"));
}

void FCompuTrainerModuleModuleImpl::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("FCompuTrainerModule::ShutdownModule"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_GAME_MODULE(FCompuTrainerModuleModuleImpl, CompuTrainerModule)