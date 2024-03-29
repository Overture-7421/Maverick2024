// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "VisionSpeakerCommandNoShoot.h"

#include <frc/MathUtil.h>

VisionSpeakerCommandNoShoot::VisionSpeakerCommandNoShoot(Chassis* chassis, SuperStructure* superStructure, Shooter* shooter) {
	// Use addRequirements() here to declare subsystem dependencies.
	AddRequirements({ superStructure, shooter });
	this->chassis = chassis;
	this->superStructure = superStructure;
	this->shooter = shooter;
}

// Called when the command is initially scheduled.
void VisionSpeakerCommandNoShoot::Initialize() {
	if (shouldFlip()) {
		targetLocation = pathplanner::GeometryUtil::flipFieldPosition(VisionSpeakerCommandConstants::TargetLocation);
	} else {
		targetLocation = VisionSpeakerCommandConstants::TargetLocation;
	}

	chassis->setHeadingOverride(true);
}

// Called repeatedly when this Command is scheduled to run
void VisionSpeakerCommandNoShoot::Execute() {
	frc::Pose2d chassisPose = chassis->getOdometry();
	frc::Translation2d chassisLoc = chassisPose.Translation();

	frc::Translation2d chassisToTarget = targetLocation - chassisLoc;
	distance = chassisToTarget.Distance({ 0_m, 0_m });
	angle = chassisToTarget.Angle().RotateBy({ -180_deg });


	chassis->setTargetHeading(angle);
	double targetLowerAngle = VisionSpeakerCommandConstants::DistanceToLowerAngleTable[distance];
	double targetUpperAngle = VisionSpeakerCommandConstants::DistanceToUpperAngleTable[distance];
	double targetShooterVelocity = VisionSpeakerCommandConstants::DistanceToVelocityTable[distance];
	superStructure->setTargetCoord({ targetLowerAngle, targetUpperAngle });
	shooter->setVelocityVoltage(targetShooterVelocity);
}

// Called once the command ends or is interrupted.
void VisionSpeakerCommandNoShoot::End(bool interrupted) {
	chassis->setHeadingOverride(false);
}

// Returns true when the command should end.
bool VisionSpeakerCommandNoShoot::IsFinished() {
	return false;
}