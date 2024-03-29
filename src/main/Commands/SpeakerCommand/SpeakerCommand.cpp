// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "SpeakerCommand.h"

// NOTE:  Consider using this command inline, rather than writing a subclass.
// For more information, see:
// https://docs.wpilib.org/en/stable/docs/software/commandbased/convenience-features.html
SpeakerCommand::SpeakerCommand(SuperStructure* superStructure, Shooter* shooter) {
  
  AddCommands(
    frc2::ParallelCommandGroup(
      SuperStructureCommand(superStructure, SuperStructureConstants::ManualSpeakerState),
      ShooterCommand(shooter, ShooterConstants::ManualSpeakerSpeed)
    )
  );
}
