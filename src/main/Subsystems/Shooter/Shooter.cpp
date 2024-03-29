// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Shooter.h"

Shooter::Shooter() {
	upperShooterMotor.setSupplyCurrentLimit(true, 20, 25, 0.5);
	upperShooterMotor.setSensorToMechanism(ShooterConstants::LowerGearboxReduction);
	upperShooterMotor.setClosedLoopVoltageRamp(0.1);

	lowerShooterMotor.setSupplyCurrentLimit(true, 20, 25, 0.5);
	lowerShooterMotor.setSensorToMechanism(ShooterConstants::UpperGearboxReduction);
	lowerShooterMotor.setClosedLoopVoltageRamp(0.1);

	upperShooterMotor.setPIDValues(0.082625, 0.0, 0.0, upperFF.kS.value(), upperFF.kV.value());
	lowerShooterMotor.setPIDValues(0.08176, 0.0, 0.0, lowerFF.kS.value(), lowerFF.kV.value());
}

void Shooter::setVelocityVoltage(double velocity) {
	if (emergencyDisabled) {
		return;
	}

	velocity = std::clamp(velocity, -ShooterConstants::MaxSpeed, ShooterConstants::MaxSpeed);
	targetVel = velocity;
	upperShooterMotor.setVelocityVoltage(velocity, 0, false);
	lowerShooterMotor.setVelocityVoltage(velocity * .8, 0, false);
}

void Shooter::setIndividualVoltage(double upper, double lower) {
	if (emergencyDisabled && upper != 0 && lower != 0) {
		return;
	}

	upperShooterMotor.SetVoltage(units::volt_t(upper));
	lowerShooterMotor.SetVoltage(units::volt_t(lower));
}

void Shooter::setEmergencyDisable(bool emergencyDisable) {
	this->emergencyDisabled = emergencyDisable;
}

bool Shooter::isEmergencyDisabled() {
	return emergencyDisabled;
}

double Shooter::getCurrentVelocity() {
	// return (getUpperMotorCurrentVelocity() + getLowerMotorCurrentVelocity()) / 2.0;
	return getUpperMotorCurrentVelocity();
}

double Shooter::getUpperMotorCurrentVelocity() {
	return upperShooterMotor.GetVelocity().GetValue().value();

}

double Shooter::getLowerMotorCurrentVelocity() {
	return lowerShooterMotor.GetVelocity().GetValue().value();

}

// This method will be called once per scheduler run
void Shooter::Periodic() {
	if (emergencyDisabled) {
		setIndividualVoltage(0, 0);
	}
}

void Shooter::shuffleboardPeriodic() {
	frc::SmartDashboard::PutNumber("Shooter/DesiredSpeed", targetVel);
	frc::SmartDashboard::PutNumber("Shooter/CurrentSpeed", getCurrentVelocity());

	frc::SmartDashboard::PutNumber("Shooter/Lower/CurrentSpeed", getLowerMotorCurrentVelocity());
	frc::SmartDashboard::PutNumber("Shooter/Upper/CurrentSpeed", getUpperMotorCurrentVelocity());
}