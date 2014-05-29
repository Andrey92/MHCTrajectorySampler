#ifndef TRAJECTORY_LOADER_H
#define TRAJECTORY_LOADER_H

#include <string>

#include "../sampler/Trajectory.h"
#include "../model/Map.h"

using namespace std;
using namespace sampler;

namespace util {

	/*
	 * ReadingsLoader class loads readings data from file.
	 *
	 * Each line of the file is in the form:
	 * 	{ri},t
	 * where {ri} is a set of comma separated reader names,
	 * and t is a time instant (integer, in milliseconds).
	 *
	 * These data are used to generate sample trajectories.
	 */

	class TrajectoryLoader {

		private:
			// Private constructor (static class)
			TrajectoryLoader() { };

		public:

			// Characters used in map file:
			// To separate values (time instants and locations)
			static const char SEP;

			// Load readings from specified file name
			// It uses a Map object to identify Readers
			static Trajectory* loadTrajectory(const string&, const Map&);

	};

}

#endif
