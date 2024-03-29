// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/controller/ArmFeedforward.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc/trajectory/TrapezoidProfile.h>

#include <OvertureLib/MotorControllers/OverTalonFX/OverTalonFX.h>
#include <OvertureLib/MotorControllers/ControllerNeutralMode/ControllerNeutralMode.h>
#include <OvertureLib/Sensors/OverDutyCycleEncoder/OverDutyCycleEncoder.h>
#include <frc2/command/sysid/SysIdRoutine.h>
#include <frc2/command/Commands.h>
#include <units/angular_velocity.h>
#include <units/angular_acceleration.h>
#include <units/angle.h>

#include <numbers>
#include <frc/estimator/KalmanFilter.h>
#include <frc/controller/LinearQuadraticRegulator.h>
#include <frc/system/LinearSystemLoop.h>
#include <frc/system/plant/LinearSystemId.h>

#include "Constants.h"
#include "SuperStructureState.h"
#include "OvertureLib/Robots/OverRobot/RobotConstants.h"

class SuperStructure : public frc2::SubsystemBase {
public:
	SuperStructure();
	void setTargetCoord(SuperStructureState targetState);
	double getLowerAngle();
	double getUpperAngle();

	void setLowerMotionMagicProfile(double p, double motionMagicSpeed, double motionMagicAccel);
	void resetLowerMotionMagic();

	frc2::CommandPtr sysIdQuasistaticLower(frc2::sysid::Direction direction) {
		return sysIdRoutineLower.Quasistatic(direction);
	}

	frc2::CommandPtr sysIdDynamicLower(frc2::sysid::Direction direction) {
		return sysIdRoutineLower.Dynamic(direction);
	}

	frc2::CommandPtr sysIdQuasistaticUpper(frc2::sysid::Direction direction) {
		return frc2::cmd::Sequence(
			frc2::cmd::Run([this] {setTargetCoord({ 0, 0 });}).WithTimeout(1.5_s),
			sysIdRoutineUpper.Quasistatic(direction)
		);
	}

	frc2::CommandPtr sysIdDynamicUpper(frc2::sysid::Direction direction) {
		return frc2::cmd::Sequence(
			frc2::cmd::Run([this] {setTargetCoord({ 0, 0 });}).WithTimeout(1.5_s),
			sysIdRoutineUpper.Dynamic(direction)
		);
	}

	SuperStructureState getCurrentState();
	void Periodic() override;
	void shuffleboardPeriodic();

private:
	// double getLowerAngleThroughBore();
	double getUpperAngleFalcon();

	void setFalconTargetPos(SuperStructureState targetState, SuperStructureState currentState);
	double convertAngleToFalconPos(double angle);

	//Encoders
	OverDutyCycleEncoder lowerEncoder{ 3 };
	OverDutyCycleEncoder upperEncoder{ 9 };

#ifdef __FRC_ROBORIO__
	double lowerOffset = -0.338472;
	double upperOffset = -0.23911290597782264;
#else
	double lowerOffset = 0;
	double upperOffset = 0;
#endif


	// LowerMotors
	OverTalonFX lowerRightMotor{ 20, ControllerNeutralMode::Brake, false, "rio" };
	OverTalonFX lowerLeftMotor{ 21, ControllerNeutralMode::Brake, true, "rio" };

	// Upper Motors
	OverTalonFX upperMotor{ 22, ControllerNeutralMode::Brake, true, "rio" };

	// State
	SuperStructureState targetState, actualTarget;
	SuperStructureState currentState;

	//Feed Forward
	frc::ArmFeedforward lowerFF{ 0.25_V, 0.4_V, 30_V / 1_tps, 0.9068_V / 1_tr_per_s_sq };
	frc::ArmFeedforward upperFF{ 0.6_V, 0.25_V, 7.5_V / 1_tps, 0.97016_V / 1_tr_per_s_sq };

	//Estimators
	frc::LinearSystem<2, 1, 1> upperArmPlant = frc::LinearSystemId::IdentifyPositionSystem<units::radian>(upperFF.kV, upperFF.kA);
	frc::KalmanFilter<2, 1, 1> upperArmObserver{ upperArmPlant, {3.0, 3.0}, {0.01}, RobotConstants::LoopTime };
	// frc::LinearQuadraticRegulator<2, 1> upperArmController { upperArmPlant, {0.1, 8.0}, {12.0}, RobotConstants::LoopTime};
	// frc::LinearSystemLoop<2, 1, 1> upperArmLoop{upperArmPlant, upperArmController, upperArmObserver, 12_V, RobotConstants::LoopTime};

	frc::ProfiledPIDController<units::degrees> lowerPID{ 0.20, 0.25, 0.0, {360_deg_per_s * 4.0, 360_deg_per_s_sq * 1.25}, RobotConstants::LoopTime };
	frc::ProfiledPIDController<units::degrees> upperPID{ 0.25, 0.3, 0.0, {360_deg_per_s * 5.0, 360_deg_per_s_sq * 1.25}, RobotConstants::LoopTime };

	units::turn_t upperFFOffset = 0.25_tr;

	double oldP = 0;
	double oldSpeed = 0;
	double oldAccel = 0;


	frc2::sysid::SysIdRoutine sysIdRoutineLower{
		frc2::sysid::Config{0.75_V / 1_s, 5_V, 10_s,
							std::nullopt},
		frc2::sysid::Mechanism{
			[this](units::volt_t driveVoltage) {
				lowerRightMotor.SetVoltage(driveVoltage);
			},
			[this](frc::sysid::SysIdRoutineLog* log) {

			log->Motor("SuperStructureLower")
				.voltage(lowerRightMotor.GetMotorVoltage().GetValue())
				.position(lowerRightMotor.GetPosition().GetValue())
				.velocity(lowerRightMotor.GetVelocity().GetValue());
			},
			this} };

	frc2::sysid::SysIdRoutine sysIdRoutineUpper{
		frc2::sysid::Config{0.75_V / 1_s, 4_V, 10_s,
							std::nullopt},
		frc2::sysid::Mechanism{
			[this](units::volt_t driveVoltage) {
				upperMotor.SetVoltage(driveVoltage);
			},
			[this](frc::sysid::SysIdRoutineLog* log) {

			log->Motor("SuperStructureUpper")
				.voltage(upperMotor.GetMotorVoltage().GetValue())
				.position(upperMotor.GetPosition().GetValue())
				.velocity(upperMotor.GetVelocity().GetValue());
			},
			this} };
};
