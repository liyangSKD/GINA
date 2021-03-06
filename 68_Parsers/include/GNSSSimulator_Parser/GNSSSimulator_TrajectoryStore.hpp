﻿#ifndef GNSSSIMULATOR_TRAJECTORY_STORE_HPP
#define GNSSSIMULATOR_TRAJECTORY_STORE_HPP

#include <iostream>
#include <map>
#include "GNSSSimulator_TrajectoryData.hpp"

namespace gnsssimulator {
	class TrajectoryStore
	{
	public:
		TrajectoryStore();
		~TrajectoryStore();

		gpstk::Position::CoordinateSystem coorSys = gpstk::Position::CoordinateSystem::Unknown;

		TrajectoryData& addPosition(TrajectoryData);
		TrajectoryData findPosition(gpstk::GPSWeekSecond);
		/* Return epochs as a vector from the Trajectoryfile.
		*/
		vector<gpstk::GPSWeekSecond> listTime(void); // TODO tudja kilist�zni a benne levo idoket.

		bool operator==(const TrajectoryStore& other) const;
		bool operator!=(const TrajectoryStore& other) const;

	protected:
		bool isCoorSystemSet = false;
		typedef std::map<gpstk::GPSWeekSecond, TrajectoryData> TrajectoryMap;
		TrajectoryMap TrajStore;
		void setCorrdinateSystem(gpstk::Position::CoordinateSystem);
		bool compare(const TrajectoryStore& other) const;
	};

}

#endif