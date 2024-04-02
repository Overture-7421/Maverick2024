// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "AmpAutoCenterRace.h"

#include "main/Commands/SpeakerCommand/SpeakerCommand.h"
#include "main/Commands/VisionAmpCommand/VisionAmpCommand.h"
#include "main/Commands/VisionSourceGrabCommand/VisionSourceGrabCommand.h"
#include "main/Commands/VisionSpeakerCommand/VisionSpeakerCommand.h"
#include "main/Commands/VisionSpeakerCommandNoShoot/VisionSpeakerCommandNoShoot.h"
#include <pathplanner/lib/auto/NamedCommands.h>

frc2::CommandPtr AmpAutoCenterRace(Storage* storage, Shooter* shooter) {
    return frc2::cmd::Sequence(
		pathplanner::NamedCommands::getCommand("VisionSpeakerCommand"),
		frc2::cmd::Parallel(
			pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto1")),
			pathplanner::NamedCommands::getCommand("GroundGrabCommand")
		),
		//Go back to shoot or grab next note if stolen
		frc2::cmd::Either(
			frc2::cmd::Sequence(
				frc2::cmd::Deadline(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto2")),
					pathplanner::NamedCommands::getCommand("VisionNoShoot")
				),
				pathplanner::NamedCommands::getCommand("VisionSpeakerCommand"),
				frc2::cmd::Parallel(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto3")),
					pathplanner::NamedCommands::getCommand("GroundGrabCommand")
				)
			),
			frc2::cmd::Sequence(
				frc2::cmd::Parallel(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("RaceAmpAuto_Stolen1")),
					pathplanner::NamedCommands::getCommand("GroundGrabCommand")
				)
			),
			[&] {return storage->isNoteOnForwardSensor();}
		),
		//Go back to shoot or grab next note if stolen
		frc2::cmd::Either(
			frc2::cmd::Sequence(
				frc2::cmd::Deadline(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto4")),
					pathplanner::NamedCommands::getCommand("VisionNoShoot")
				),
				pathplanner::NamedCommands::getCommand("VisionSpeakerCommand"),
				frc2::cmd::Parallel(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto5")),
					pathplanner::NamedCommands::getCommand("GroundGrabCommand")
				)
			),
			frc2::cmd::Sequence(
				frc2::cmd::Parallel(
					pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("RaceAmpAuto_Stolen2")),
					pathplanner::NamedCommands::getCommand("GroundGrabCommand")
				)
			),
			[&] {return storage->isNoteOnForwardSensor();}
		),
		frc2::cmd::Deadline(
			pathplanner::AutoBuilder::followPath(pathplanner::PathPlannerPath::fromPathFile("AMPAuto6")),
			pathplanner::NamedCommands::getCommand("VisionNoShoot")
		),
		pathplanner::NamedCommands::getCommand("VisionSpeakerCommand")
	);
}

