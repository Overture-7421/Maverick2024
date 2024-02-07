// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/CommandHelper.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/XboxController.h>

#include "OvertureLib/Math/InterpolatingTable/InterpolatingTable.h"
#include "OvertureLib/Math/Utils.h"
#include "OvertureLib/Math/TargetingWhileMoving/TargetingWhileMoving.h"
#include "main/Subsystems/Chassis/Chassis.h"
#include "main/Subsystems/SuperStructure/SuperStructure.h"
#include "main/Subsystems/Shooter/Shooter.h"

#include "main/Commands/UtilityFunctions/UtilityFunctions.h"

/**
 * An example command.
 *
 * <p>Note that this extends CommandHelper, rather extending Command
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class VisionSpeakerCommand
    : public frc2::CommandHelper<frc2::Command, VisionSpeakerCommand> {
 public:
  VisionSpeakerCommand(Chassis* chassis, SuperStructure* SuperStructure, Shooter* shooter, frc::XboxController* joystick);

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

 private:
  InterpolatingTable<units::meter_t, double> distanceToUpperAngleTable{
    {0_m, -1.00},
    {1_m, -5.00},
    {2_m, -15.00},
    {3_m, -20.00},
    {4_m, -25.00},
    {5_m, -30.00},
    {6_m, -35.00}
    };

  InterpolatingTable<units::meter_t, double> distanceToLowerAngleTable{
    {1_m, 1.00},
    {2_m, 3.00},
    {3_m, 5.00},
    {4_m, 7.00},
    {5_m, 9.00},
    {6_m, 11.00},
    {7_m, 13.00}
    };

  InterpolatingTable<units::meter_t, double> distanceToVelocityTable{
    {0.1_m, 1.00},
    {0.2_m, 2.00},
    {0.3_m, 3.00},
    {0.4_m, 4.00},
    {0.5_m, 5.00},
    {0.6_m, 6.00},
    {0.7_m, 7.00}
    };

  InterpolatingTable<units::meter_t, double> DistanceToTravelTimeTable{
    {0.1_m, 1.00},
    {0.2_m, 2.00},
    {0.3_m, 3.00},
    {0.4_m, 4.00},
    {0.5_m, 5.00},
    {0.6_m, 6.00},
    {0.7_m, 7.00}
    };

  SuperStructure* superStructure;
  Chassis* chassis;
  Shooter* shooter;
  frc::XboxController* joystick;

  TargetingWhileMoving dynamicTarget {
    flipTranslationIfNeeded({0.06_m, 5.54_m}), 
    {
    {1_m, 0.8_s},
    {2_m, 0.8_s},
    {3_m, 0.8_s},
    {4_m, 0.8_s},
    {5_m, 0.8_s},
    {6_m, 0.8_s},
    {7_m, 0.8_s}
    }
  };
  units::meter_t distance = 0.0_m;
  frc::Rotation2d angle;
};