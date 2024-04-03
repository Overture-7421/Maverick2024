// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "TargetProvider.h"
TargetProvider::TargetProvider(frc::AprilTagFieldLayout* tagLayout) {
    this->tagLayout = tagLayout;
}

frc::Translation2d TargetProvider::GetSpeakerLocation() {
    if (isRedAlliance()) {
        return tagLayout->GetTagPose(4).value().ToPose2d().Translation();
    } else {
        return tagLayout->GetTagPose(7).value().ToPose2d().Translation();
    }
};

frc::Translation2d 	TargetProvider::GetPassLocation() {
	if (isRedAlliance()) {
		return { 15.32_m, 7.11_m };
	} else {
		return { 1.01_m, 7.11_m };
	}
}

