// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>

RobotContainer::RobotContainer() {
	SuperStructureState startingState{ 0,0 };
	SuperStructureState targetState{ 40, 40 };

	SuperStructureMoveByDistance::Profile profile;
	profile.profileActivationDistance = 1_m;
	profile.startingState = startingState;
	profile.targetState = targetState;
	profile.targetCoord = frc::Translation2d(4.3_m, 5.0_m);

	pathplanner::NamedCommands::registerCommand("Climb", std::move(SuperStructureMoveByDistance(&chassis, &superStructure, profile).ToPtr()));

	autoChooser.SetDefaultOption("None, null, nada", "None");
	autoChooser.AddOption("MiddleNote", "MiddleNote");

	frc::SmartDashboard::PutData("Auto Chooser", &autoChooser);
	ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
	chassis.SetDefaultCommand(Drive(&chassis, &driver));

	// Configure the button bindings
	resetAngleButton.WhileTrue(ResetAngle(&chassis).ToPtr());


	intakeTrue.OnTrue(IntakeCommand(&intake, 3_V).ToPtr());
	intakeTrue.OnFalse(IntakeCommand(&intake, 0_V).ToPtr());
	storageTrue.OnTrue(StorageCommand(&storage, 2_V).ToPtr());
	storageTrue.OnFalse(StorageCommand(&storage, 0_V).ToPtr());
	storageFalse.OnTrue(StorageCommand(&storage, -2_V).ToPtr());
	storageFalse.OnFalse(StorageCommand(&storage, 0_V).ToPtr());
	superStructurePos1.WhileTrue(SuperStructureCommand(&superStructure, {0.0, 0.0}).ToPtr());
	superStructurePos2.WhileTrue(SuperStructureCommand(&superStructure, {30.0, -30.0}).ToPtr());
	supportArmOpen.OnTrue(SupportArmCommand2(&supportArms, {-90.0}).ToPtr())
					.OnFalse(SupportArmCommand2(&supportArms, {0.0}).ToPtr());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand() {
	std::string autoName = autoChooser.GetSelected();
	if (autoName == "None") {
		return  frc2::cmd::None();
	}

	return pathplanner::AutoBuilder::buildAuto(autoName);
}
