#include <string>
#include <fstream>

#include "../include/util/TrajectoryLoader.h"
#include "../include/util/Util.h"
#include "../include/exceptions/BadInput.h"

using namespace std;
using namespace exceptions;

namespace util {

	/*
	 * Implementation of TrajectoryLoader utility class.
	 */

	const char TrajectoryLoader::SEP = ' ';

	Trajectory* TrajectoryLoader::loadTrajectory(const string& fileName, const Map& map) {
		NameIdMap locationsMap;
		for (auto it = map.lbegin(); it != map.lend(); it++) {
			Location l = *it;
			locationsMap.insert(NameIdPair(l.getName(), l.getId()));
		}
		ifstream trajectoryFile(fileName);
		string line;
		unsigned long count = 0;
		Trajectory* t;
		if (trajectoryFile.is_open()) {
			while (getline(trajectoryFile, line)) count++;
			trajectoryFile.clear();
			trajectoryFile.seekg(0, trajectoryFile.beg);
			t = new Trajectory(0, count);
			count = 0;
			while (getline(trajectoryFile, line)) {
				Util::trim(line);
				// Jump if blank line or comment (starts with '#')
				if (line.length() == 0 || line[0] == '#') continue;
				vector<string> v = Util::split(line, SEP);
				if (v.size() != 2) throw BadInput();
				t->setLocation(locationsMap.at(v[1]), count++, 1.0);
			}
		} else throw BadInput();
		return t;
	}

}
