// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>

#include "main/Subsystems/Chassis/Chassis.h"
#include "main/Subsystems/SuperStructure/SuperStructure.h"
#include "main/Subsystems/Shooter/Shooter.h"
#include "main/Commands/VisionSpeakerCommand/Constants.h"
#include "main/Commands/UtilityFunctions/UtilityFunctions.h"

class VisionSpeakerCommandNoShoot
	: public frc2::CommandHelper<frc2::Command, VisionSpeakerCommandNoShoot> {
public:
	VisionSpeakerCommandNoShoot(Chassis* chassis, SuperStructure* SuperStructure, Shooter* shooter);

	void Initialize() override;

	void Execute() override;

	void End(bool interrupted) override;

	bool IsFinished() override;

private:
	SuperStructure* superStructure;
	Chassis* chassis;
	Shooter* shooter;
	
	frc::Translation2d targetLocation;

	units::meter_t distance = 0.0_m;
	frc::Rotation2d angle;
};