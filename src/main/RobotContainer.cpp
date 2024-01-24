// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>

RobotContainer::RobotContainer() {

	autoChooser.SetDefaultOption("None, null, nada", "None");
	autoChooser.AddOption("MiddleNote", "MiddleNote");

	frc::SmartDashboard::PutData("Auto Chooser", &autoChooser);
	ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
	chassis.SetDefaultCommand(Drive(&chassis, &driver));
	superStructure.SetDefaultCommand(IdolStart(&intake, &superStructure, &storage, &shooter));

	// Configure the button bindings
	resetAngleButton.WhileTrue(ResetAngle(&chassis).ToPtr());

	intakePosition.OnTrue(StartIntake(&intake, &superStructure, &storage)).OnFalse(StopIntake(&intake, &superStructure, &storage));
	shootingPose.OnTrue(ShootingPose(&intake, &superStructure)).OnFalse(StopIntake(&intake, &superStructure, &storage));
	moveStorage.WhileTrue(MoveStorage(&storage, 1_V).ToPtr());
	moveStorageInverted.WhileTrue(MoveStorage(&storage, -1_V).ToPtr());
	shootshooter.WhileTrue(ShootShooter(&shooter, 3.0).ToPtr());
	shooterAngle.WhileTrue(ShooterAngle(&superStructure));
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
	std::string autoName = autoChooser.GetSelected();
	if (autoName == "None") {
		return  frc2::cmd::None();
	}

	return pathplanner::AutoBuilder::buildAuto(autoName);
}
