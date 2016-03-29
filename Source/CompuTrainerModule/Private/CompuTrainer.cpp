// Fill out your copyright notice in the Description page of Project Settings.

#include "CompuTrainerPluginModulePCH.h"
#include "CompuTrainer.h"

#if WITH_COMPUTRAINER_BINDING
#include "RacerMate/racermate.h"
#endif

void UCompuTrainer::Initialize(int32 ComPort, int32 DefaultResistance)
{
	Port = ComPort - 1;

#if WITH_COMPUTRAINER_BINDING
	int status = GetRacerMateDeviceID(Port);
	if (status != DEVICE_COMPUTRAINER)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Failed to find CompuTrainer on ComPort %d."), (Port + 1));
	}

	FirmwareVersion = (int32)GetFirmWareVersion(Port);
	if (FirmwareVersion < 0)
	{
		const char *pErrorString = get_errstr(FirmwareVersion);
		UE_LOG(LogTemp, Fatal, TEXT("Failed to get Firmware Version from CompuTrainer: %s"), pErrorString);
	}

	bool isCalibrated = GetIsCalibrated(Port, FirmwareVersion) == 0 ? false : true;
	if (!isCalibrated)
	{
		UE_LOG(LogTemp, Log, TEXT("CompuTrainer is not calibrated."));
	}

	const char* pAPIVersion = GetAPIVersion();
	UE_LOG(LogTemp, Log, TEXT("CompuTrainer API Version: %s"), pAPIVersion);

	const char* pDLLVersion = get_dll_version();
	UE_LOG(LogTemp, Log, TEXT("CompuTrainer DLL Version: %s"), pDLLVersion);
	
	if (isCalibrated)
	{
		Calibration = (int32)GetCalibration(Port);
		if (Calibration < 0)
		{
			const char *pErrorString = get_errstr((int)Calibration);
			UE_LOG(LogTemp, Fatal, TEXT("Failed to get Calibration from CompuTrainer: %s (Try power toggling CompuTrainer!)"), pErrorString);
		}
	}
	else
	{
		Calibration = DefaultResistance;
	}	

	status = startTrainer(Port);
	if (status != ALL_OK)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Failed to run Start Trainer on CompuTrainer"));
	}

	UE_LOG(LogTemp, Log, TEXT("CompuTrainer started on port: %d"), (Port + 1));
	//FPlatformProcess::Sleep(1);
 
	status = resetTrainer(Port, FirmwareVersion, Calibration);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Fatal, TEXT("Failed to run Reset Trainer on CompuTrainer: %s"), pErrorString);
	}

	status = ResetAverages(Port, FirmwareVersion);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Fatal, TEXT("Failed to run Reset Averages on CompuTrainer: %s"), pErrorString);
	}

	status = setPause(Port, true);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Fatal, TEXT("Failed to pause CompuTrainer on startup: %s"), pErrorString);
	}
	//FPlatformProcess::Sleep(0.5f);

	status = setPause(Port, false);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Fatal, TEXT("Failed to unpause CompuTrainer on startup: %s"), pErrorString);
	}
#endif

	UE_LOG(LogTemp, Log, TEXT("CompuTrainer finished initialization."));
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UCompuTrainer::UpdateFromHardware), 0.1f, true);
}

void UCompuTrainer::UpdateFromHardware()
{
	FCompuTrainerData ueData;
#if WITH_COMPUTRAINER_BINDING
	TrainerData trainerData = GetTrainerData(Port, FirmwareVersion);

	// Copy the data over..
	ueData.KilometersPerHour = trainerData.kph;
	ueData.Cadence = trainerData.cadence;
	ueData.HeartRate = trainerData.HR;
	ueData.PowerInWatts = trainerData.Power;

	// Check our latch state of the keys on the CompuTrainer so we can broadcast events.
	int keys = GetHandleBarButtons(Port, FirmwareVersion);
	
	switch (keys)
	{
	case 0x01:
		if(OnResetPressed.IsBound()) OnResetPressed.Broadcast();
		break;
	case 0x04:
		if (OnF2Pressed.IsBound()) OnF2Pressed.Broadcast();
		break;
	case 0x10:
		if(OnPlusPressed.IsBound()) OnPlusPressed.Broadcast();
		break;
	case 0x02:
		if(OnF1Pressed.IsBound()) OnF1Pressed.Broadcast();
		break;
	case 0x08:
		if(OnF3Pressed.IsBound()) OnF3Pressed.Broadcast();
		break;
	case 0x20:
		if(OnMinusPressed.IsBound()) OnMinusPressed.Broadcast();
		break;
	}

#else
	ueData.Cadence = 68; // Average biking speed
	ueData.HeartRate = 0;
	ueData.PowerInWatts = -1;
	// Converting to KPH since we don't set these in KPH.
	ueData.KilometersPerHour = (((SpeedProvidedByBlueprint / 1000.0f) * 60.0f) * 60.0f);
#endif

	if (OnDataUpdateNotify.IsBound())
	{
		OnDataUpdateNotify.Broadcast(ueData);
	}
}

void UCompuTrainer::BeginDestroy()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
 		UE_LOG(LogTemp, Log, TEXT("Destroying CompuTrainer %s on Port %d"), *GetOuter()->GetFullName(), Port);
	#if WITH_COMPUTRAINER_BINDING

 		int status = stopTrainer(Port);
		if (status != ALL_OK)
		{
			const char *pErrorString = get_errstr(status);
			//UE_LOG(LogTemp, Fatal, TEXT("Failed to run Stop Trainer on CompuTrainer: %s"), pErrorString);
		}

		status = ResettoIdle(Port);
		if (status != ALL_OK)
		{
			const char *pErrorString = get_errstr(status);
			//UE_LOG(LogTemp, Fatal, TEXT("Failed to run Stop Trainer on CompuTrainer: %s"), pErrorString);
		}
	#endif
	}
	
	Super::BeginDestroy();
}

void UCompuTrainer::SetErgonomicMode(float loadInWatts)
{
#if WITH_COMPUTRAINER_BINDING
	int status = SetErgModeLoad(Port, FirmwareVersion, Calibration, loadInWatts);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Log, TEXT("Failed to set Ergonomic Mode Setting: %s"), pErrorString);
	}
#endif
}

void UCompuTrainer::SetSlopeMode(float BikeWeightInKg, float RiderWeightInKg, int32 DragFactor, float Slope)
{
#if WITH_COMPUTRAINER_BINDING
	int status = SetSlope(Port, FirmwareVersion, Calibration, BikeWeightInKg, RiderWeightInKg, DragFactor, Slope);
	if (status != ALL_OK)
	{
		const char *pErrorString = get_errstr(status);
		UE_LOG(LogTemp, Log, TEXT("Failed to set Slope Mode Setting: %s"), pErrorString);
	}
#endif
}

void UCompuTrainer::DEBUG_AddSpeed(float SpeedInMetersPerSecond)
{
#if !WITH_COMPUTRAINER_BINDING
	SpeedProvidedByBlueprint += SpeedInMetersPerSecond;
#endif
}
