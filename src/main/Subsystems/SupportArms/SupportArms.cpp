// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "SupportArms.h"
#include <frc/MathUtil.h>
#include <thread>

#define DEG_TO_RAD M_PI / 180.0

SupportArms::SupportArms() {
	// // Configure Motors
	// m_lowerRight.setSupplyCurrentLimit(true, 20, 30, 0.5);
	// m_lowerRight.setSensorToMechanism(LOWER_GEAR_BOX_REDUCTION);

	// // COnfigure Motion Magic and PID
	// m_lowerRight.setPIDValues(45.0, 0.0, 0.0, 0.0, 0.0);
	// m_lowerRight.configureMotionMagic(20.0, 20.0, 0.0);

	// std::this_thread::sleep_for(std::chrono::seconds(2));
	// m_lowerRight.setSensorPosition(convertAngleToFalconPos(getLowerAngle()));


	// Configure Motors
	m_lowerRight.SetSmartCurrentLimit(20);
	m_lowerRight.SetSecondaryCurrentLimit(30);

	// Configure Motion Magic and PID
	pidController.SetP(0.0);
	pidController.SetI(0.0);
	pidController.SetD(0.0);
	pidController.SetFF(0.0);

	pidController.SetSmartMotionMaxVelocity(0.0);
	pidController.SetSmartMotionMaxAccel(0.0);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	encoder.SetPositionConversionFactor(1.0 / LOWER_GEAR_BOX_REDUCTION);
	encoder.SetPosition(convertAngleToFalconPos(getLowerAngle()));

	setTargetCoord({ getLowerAngle() });
}

void SupportArms::setTargetCoord(SupportArmsState targetCoord) {
	m_TargetState = targetCoord;
}

double SupportArms::getLowerAngle() {
	return (lowerEncoder.GetAbsolutePosition() - lowerOffset) * 360;
}

SupportArmsPosition SupportArms::getPosition() {
	return position;
}

void SupportArms::setPosition(SupportArmsPosition pos) {
	position = pos;
}

SupportArmsState SupportArms::getCurrentState() {
	SupportArmsState state;
	state.lowerAngle = getLowerAngle();
	return state;
}

void SupportArms::setFalconTargetPos(SupportArmsState targetState, SupportArmsState currentState) {
	// m_lowerRight.setMotionMagicPosition(convertAngleToFalconPos(targetState.lowerAngle), lowerFF * cos(currentState.lowerAngle * DEG_TO_RAD), false);
	setpoint = trapezoidProfile.Calculate(dt, setpoint, {units::degree_t(targetState.lowerAngle), 0_deg_per_s});
	
	pidController.SetReference(convertAngleToFalconPos(setpoint.position.value()), rev::ControlType::kPosition, 0, lowerFF * cos(currentState.lowerAngle * DEG_TO_RAD));
}

double SupportArms::convertAngleToFalconPos(double angle) {
	return angle / 360;
}

// This method will be called once per scheduler run
void SupportArms::Periodic() {
	SupportArmsState currentState = getCurrentState();
	setFalconTargetPos(m_TargetState, currentState);


	// Debugging
	frc::SmartDashboard::PutNumber("SupportArms/Current/Lower Angle", currentState.lowerAngle);

	frc::SmartDashboard::PutNumber("SupportArms/Current/Lower Position", encoder.GetPosition());
}
