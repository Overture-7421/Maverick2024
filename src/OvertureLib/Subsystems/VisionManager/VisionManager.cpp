// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "VisionManager.h"

VisionManager::VisionManager() {};
void VisionManager::setCameraAndLayout(photon::PhotonCamera* camera, frc::AprilTagFieldLayout* tagLayout, frc::Transform3d* cameraToRobot) {
	this->m_Camera = camera;
	this->m_TagLayout = tagLayout;
	this->m_CameraToRobot = cameraToRobot;

	m_TagLayout->SetOrigin(frc::AprilTagFieldLayout::OriginPosition::kBlueAllianceWallRightSide);

	poseEstimatorSet = true;
	poseEstimator = new photon::PhotonPoseEstimator{
		*m_TagLayout,
		photon::PoseStrategy::MULTI_TAG_PNP_ON_COPROCESSOR,
		std::move(photon::PhotonCamera{ "Arducam_OV9281_USB_Camera" }),
		*m_CameraToRobot
	};
}

//Check if distance between robot and tag is less than a certain value
bool VisionManager::checkTagDistance(size_t numberOfTags, double distance) {
	std::optional<photon::PhotonPipelineResult> result = getCameraResult();

	if (result.has_value()) {
		photon::PhotonPipelineResult resultValue = result.value();
		if (resultValue.GetTargets().size() == numberOfTags) {
			if (resultValue.GetBestTarget().GetBestCameraToTarget().X().value() < distance) {
				return true;
			}
		}
	}

	return false;
}

void VisionManager::addMeasurementToChassis() {
	std::optional<photon::EstimatedRobotPose> poseResult = update(swerveChassis->getOdometry());

	if (poseResult.has_value()) {
		frc::Pose2d poseTo2d = poseResult.value().estimatedPose.ToPose2d();
		swerveChassis->addVisionMeasurement({ poseTo2d.X(), poseTo2d.Y(), swerveChassis->getOdometry().Rotation() }, poseResult.value().timestamp);
	}
}

//Update odometry with vision

void VisionManager::updateOdometry() {
	if (checkTagDistance(1, 5.00) || checkTagDistance(2, 7.00) || checkTagDistance(3, 8.00)) {
		addMeasurementToChassis();
	}
}

//Get EstimatedRobotPose from PhotonVision
std::optional<photon::EstimatedRobotPose> VisionManager::update(frc::Pose2d estimatedPose) {
	return poseEstimator->Update();
}

//Get PhotonPipeResult from PhotonVision
std::optional<photon::PhotonPipelineResult> VisionManager::getCameraResult() {
	return m_Camera->GetLatestResult();
}

//Check if poseEstimator is set
bool VisionManager::isPoseEstimatorSet() {
	return poseEstimatorSet;
}

void VisionManager::setPoseEstimator(bool set) {
	poseEstimatorSet = set;
}

void VisionManager::Periodic() {
	// frc::SmartDashboard::PutBoolean("Set Camara", isPoseEstimatorSet());
	if (isPoseEstimatorSet()) {
		updateOdometry();
	}
}

