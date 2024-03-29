// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Drive.h"
#include <iostream>

Drive::Drive(SwerveChassis* swerveChassis, frc::XboxController* controller) :
	m_swerveChassis(swerveChassis), joystick(controller) {
	// Use addRequirements() here to declare subsystem dependencies.
	AddRequirements(m_swerveChassis);
}

// Called when the command is initially scheduled.
void Drive::Initialize() {
	if (frc::DriverStation::GetAlliance() == frc::DriverStation::Alliance::kRed){
		alliance = -1;
	}else{
		alliance = 1;
	}
}

// Called repeatedly when this Command is scheduled to run
void Drive::Execute() {
	units::meters_per_second_t xInput{ Utils::ApplyAxisFilter(-joystick->GetLeftY()) * kMaxSpeed};
	units::meters_per_second_t yInput{ Utils::ApplyAxisFilter(-joystick->GetLeftX()) * kMaxSpeed};
	units::radians_per_second_t rInput{ Utils::ApplyAxisFilter(-joystick->GetRightX()) * kMaxAngularSpeed};

	if (joystick->GetLeftBumper()) {
		m_swerveChassis->driveRobotRelative({
			xLimiter.Calculate(xInput),
			yLimiter.Calculate(yInput),
			rLimiter.Calculate(rInput) });
		} else {
			m_swerveChassis->driveFieldRelative({
				xLimiter.Calculate(xInput * alliance),
				yLimiter.Calculate(yInput * alliance),
				rLimiter.Calculate(rInput) });
	}
}

// Called once the command ends or is interrupted.
void Drive::End(bool interrupted) {}

// Returns true when the command should end.
bool Drive::IsFinished() {
	return false;
};