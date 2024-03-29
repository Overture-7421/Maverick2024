// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Storage.h"

Storage::Storage() {
	storageMotor.setSupplyCurrentLimit(true, 20, 30, 0.5);
}

void Storage::setVoltage(units::volt_t voltage) {
	storageMotor.setVoltage(voltage, false);
}

bool Storage::isNoteOnForwardSensor() {
	// return !forwardSensor.Get();
	return noteOnForwardCache;
}

bool Storage::isNoteOnBackSensor() {
	return !backSensor.Get();
}

// This method will be called once per scheduler run
void Storage::Periodic() {
	noteOnForwardCache = m_debouncer.Calculate(!forwardSensor.Get());
	// frc::SmartDashboard::PutBoolean("Storage/NoteOnBack", isNoteOnBackSensor());
}

void Storage::shuffleboardPeriodic() {
	frc::SmartDashboard::PutBoolean("Storage/NoteOnForward", isNoteOnForwardSensor());
}
