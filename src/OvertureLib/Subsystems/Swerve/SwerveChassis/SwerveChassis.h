// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Translation2d.h>
#include <frc/kinematics/SwerveDriveKinematics.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/SwerveDriveOdometry.h>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/estimator/SwerveDrivePoseEstimator.h>
#include <frc/DriverStation.h>
#include <frc/RobotController.h>
#include <frc/filter/LinearFilter.h>
#include <frc/smartdashboard/Field2d.h>
#include <frc/controller/ProfiledPIDController.h>

#include <frc2/command/SubsystemBase.h>
#include <pathplanner/lib/auto/AutoBuilder.h>
#include <pathplanner/lib/util/HolonomicPathFollowerConfig.h>
#include <pathplanner/lib/util/PIDConstants.h>
#include <pathplanner/lib/util/ReplanningConfig.h>
#include <pathplanner/lib/controllers/PPHolonomicDriveController.h>

#include "OvertureLib/Sensors/OverPigeon/OverPigeon.h"
#include "OvertureLib/Subsystems/Swerve/SwerveModule/SwerveModule.h"
#include "OvertureLib/Math/ChassisAccels.h"

#include "OvertureLib/Robots/OverRobot/RobotConstants.h"


using namespace pathplanner;

class SwerveChassis : public frc2::SubsystemBase {
public:
	SwerveChassis();
	void setTargetHeading(frc::Rotation2d rotationTarget);
	void setHeadingOverride(bool headingOverride);
	void setModulePositions(std::array<frc::Translation2d, 4>* positions);
	void setModulesRatios(double turnRatio, double driveRatio, double wheelDiameter);
	void setModules(SwerveModule* frontLeft, SwerveModule* frontRight, SwerveModule* backleft, SwerveModule* backRight);
	void setRotatorPID(double kP, double kI, double kD);
	void setDrivePID(double kP, double kI, double kD);
	void setFeedForward(units::volt_t kS, units::volt_t kV, units::volt_t kA);

	void driveRobotRelative(frc::ChassisSpeeds speeds);
	void driveFieldRelative(frc::ChassisSpeeds speeds);
	frc::ChassisSpeeds getRobotRelativeSpeeds();
	frc::ChassisSpeeds getFieldRelativeSpeeds();
	ChassisAccels getFIeldRelativeAccels();

	const frc::Pose2d& getOdometry();
	void resetOdometry(frc::Pose2d initPose);
	const frc::SwerveDriveKinematics<4>& getKinematics();
	void addVisionMeasurement(frc::Pose2d pose, units::second_t Latency);
	void resetAngle(double angle = 0);

	void setModuleStates(wpi::array<frc::SwerveModuleState, 4> desiredStates);
	wpi::array<frc::SwerveModuleState, 4> getModuleStates();
	wpi::array<frc::SwerveModulePosition, 4> getModulePosition();

	double getPitch();
	double getYaw();
	double getRoll();

	void updateOdometry();
	void shuffleboardPeriodic();
	void Periodic() override;

protected:
	OverPigeon* pigeon;

	SwerveModule* frontLeftModule;
	SwerveModule* frontRightModule;
	SwerveModule* backLeftModule;
	SwerveModule* backRightModule;

	std::unique_ptr<frc::SwerveDriveKinematics<4>> kinematics;

	std::array<frc::SwerveModulePosition, 4> odometryPos;

	std::unique_ptr<frc::SwerveDrivePoseEstimator<4>> odometry;
	frc::Pose2d latestPose;

	frc::ChassisSpeeds desiredSpeeds;

	frc::ChassisSpeeds fieldRelativeSpeed, lastFieldRelativeSpeed;
	ChassisAccels fieldRelativeAccel;
	frc::LinearFilter<units::meters_per_second_squared_t> accelXFilter = frc::LinearFilter<units::meters_per_second_squared_t>::SinglePoleIIR(0.05, RobotConstants::LoopTime);
	frc::LinearFilter<units::meters_per_second_squared_t> accelYFilter = frc::LinearFilter<units::meters_per_second_squared_t>::SinglePoleIIR(0.05, RobotConstants::LoopTime);
	frc::LinearFilter<units::radians_per_second_squared_t> accelOmegaFilter = frc::LinearFilter<units::radians_per_second_squared_t>::SinglePoleIIR(0.05, RobotConstants::LoopTime);

private:
	std::optional<frc::Rotation2d> getRotationTargetOverride();

	frc::Field2d field2d;

	bool headingOverride = false;

	frc::ProfiledPIDController<units::radians> headingController{ 11.0, 0.5, 0.3, frc::TrapezoidProfile<units::radians>::Constraints{ 18_rad_per_s, 18_rad_per_s_sq * 6 }, RobotConstants::LoopTime };
	frc::Rotation2d headingTarget;
};