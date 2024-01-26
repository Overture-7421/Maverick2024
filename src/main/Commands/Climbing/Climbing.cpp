#include "Climbing.h"
#include <exception>

#include <iostream>

ClimbingLocation findClosestClimbingLocation(Chassis* chassis) {
	std::vector<std::pair<ClimbingLocation, units::meter_t>> distancesToClimbingLocations;
	distancesToClimbingLocations.reserve(3);

	auto alliance = frc::DriverStation::GetAlliance();
	auto shouldFlip = alliance.has_value() && (alliance.value() == frc::DriverStation::Alliance::kRed);

	for(auto location : climbingLocations) {
		if(shouldFlip){
			distancesToClimbingLocations.push_back(std::pair{location.first, chassis->getOdometry().Translation().Distance(pathplanner::GeometryUtil::flipFieldPosition(location.second))});
		} else {
			distancesToClimbingLocations.push_back(std::pair{location.first, chassis->getOdometry().Translation().Distance(location.second)});
		}
	}

	std::sort(distancesToClimbingLocations.begin(), distancesToClimbingLocations.end(), [](auto a, auto b) { return a.second < b.second;});

	return distancesToClimbingLocations.front().first;
}

frc2::CommandPtr Climb(Chassis* chassis) {
	std::shared_ptr<pathplanner::PathPlannerPath> climbPathLeft = pathplanner::PathPlannerPath::fromPathFile("AMP Climb Left");
	std::shared_ptr<pathplanner::PathPlannerPath> climbPathRight = pathplanner::PathPlannerPath::fromPathFile("AMP Climb Right");
	std::shared_ptr<pathplanner::PathPlannerPath> climbPathBack = pathplanner::PathPlannerPath::fromPathFile("AMP Climb Back");

	pathplanner::PathConstraints constraints = pathplanner::PathConstraints(
		4.0_mps, 2.0_mps_sq,
		560_deg_per_s, 720_deg_per_s_sq);

	return frc2::cmd::Select<ClimbingLocation>([chassis]() {return findClosestClimbingLocation(chassis);},
		std::pair{ClimbingLocation::Left, pathplanner::AutoBuilder::pathfindThenFollowPath(climbPathLeft, constraints)},
		std::pair{ClimbingLocation::Right, pathplanner::AutoBuilder::pathfindThenFollowPath(climbPathRight, constraints)},
		std::pair{ClimbingLocation::Back, pathplanner::AutoBuilder::pathfindThenFollowPath(climbPathBack, constraints)}
	);
};

std::ostream& operator<< (std::ostream& os, ClimbingLocation location)
{
    switch (location)
    {
        case ClimbingLocation::Left : return os << "Left" ;
        case ClimbingLocation::Right: return os << "Right";
        case ClimbingLocation::Back: return os << "Back";
        // omit default case to trigger compiler warning for missing cases
    };
    return os << static_cast<std::uint16_t>(location);
}