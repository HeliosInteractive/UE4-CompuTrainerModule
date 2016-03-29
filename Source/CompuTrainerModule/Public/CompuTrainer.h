// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "CompuTrainer.generated.h"


USTRUCT(BlueprintType)
struct FCompuTrainerData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompuTrainer")
	float KilometersPerHour; // < 0 on Error

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompuTrainer")
	float Cadence; // RPM, < 0 if not connected or errored

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompuTrainer")
	float HeartRate; // BPM, < 0 if not connected or errored

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompuTrainer")
	float PowerInWatts; // 0 on error.

	FCompuTrainerData() { }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompuTrainerButtonPressedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompuTrainerDataUpdateDelegate, FCompuTrainerData, Data);

UCLASS(BlueprintType)
class UCompuTrainer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CompuTrainer")
		void Initialize(int32 ComPort, int32 DefaultResistance = 372);

	UFUNCTION(BlueprintCallable, Category = "CompuTrainer")
		void SetErgonomicMode(float loadInWatts);

	UFUNCTION(BlueprintCallable, Category = "CompuTrainer")
		void SetSlopeMode(float BikeWeightInKg, float RiderWeightInKg, int32 DragFactor, float Slope);

	// Debug for Editor Use Only
	UFUNCTION(BlueprintCallable, Category = "CompuTrainer")
	void DEBUG_AddSpeed(float SpeedInMetersPerSecond);

	void UpdateFromHardware();

	virtual void BeginDestroy() override;



public:
	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerDataUpdateDelegate OnDataUpdateNotify;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnResetPressed;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnF1Pressed;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnF2Pressed;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnF3Pressed;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnPlusPressed;

	UPROPERTY(BlueprintAssignable, Category = "CompuTrainer")
		FOnCompuTrainerButtonPressedDelegate OnMinusPressed;


private:
	int32		FirmwareVersion;
	int32		Calibration;
	int32		Port;

	FTimerHandle	TimerHandle;


	// Debug for Editor Use Only
	float		SpeedProvidedByBlueprint;
};
