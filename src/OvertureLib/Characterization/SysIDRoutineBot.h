// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>

#include "OvertureLib/Subsystems/Swerve/SwerveCharacterization/SwerveCharacterization.h"

#include "OvertureLib/Commands/Drive/Drive.h"

class SysIDRoutineBot {
public:
	SysIDRoutineBot();

protected:
	void ConfigureSysIdBindings(SwerveCharacterization* m_drive, frc2::CommandXboxController* m_driverController);
};
