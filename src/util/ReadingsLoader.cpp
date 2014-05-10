#include <string>
#include <fstream>

#include "../include/util/ReadingsLoader.h"
#include "../include/util/Util.h"
#include "../include/exceptions/BadInput.h"

using namespace std;
using namespace exceptions;

namespace util {

	/*
	 * Implementation of DataLoader utility class.
	 */

	const char ReadingsLoader::SEP = ',';
	const char ReadingsLoader::STARTSET = '{';
	const char ReadingsLoader::ENDSET = '}';

	Readings* ReadingsLoader::loadReadings(const string& fileName, const Map& map) {
		NameIdMap readersMap;
		for (auto it = map.rbegin(); it != map.rend(); it++) {
			Reader r = *it;
			readersMap.insert(NameIdPair(r.getName(), r.getId()));
		}
		ifstream readingsFile(fileName);
		string line;
		Readings* r = new Readings();
		if (readingsFile.is_open()) {
			while (getline(readingsFile, line)) {				
				Util::trim(line);
				// Jump if blank line or comment (starts with '#')
				if (line.length() == 0 || line[0] == '#') continue;
				vector<string> v = Util::split(line, ENDSET);
				if (v.size() != 2) throw BadInput();
				vector<string> v2 = Util::split(v[0], STARTSET);
				list<unsigned int> rs;
				if (v2.size() == 1) {
					vector<string> names = Util::split(v2[0], SEP);
					for (auto it = names.cbegin(); it != names.cend(); it++) {
						rs.push_back(readersMap.at(*it));
					}
				} else if (v2.size() != 0) {
					throw BadInput();
				}
				r->push_back(Reading(rs, stoul(Util::split(v[1], SEP)[0])));
			}
		} else throw BadInput();
		return r;
	}

}
