#include <string>
#include <fstream>
#include <list>
#include <unordered_map>

#include "../include/model/Location.h"
#include "../include/model/Reader.h"
#include "../include/model/Map.h"
#include "../include/util/MapLoader.h"
#include "../include/util/Util.h"
#include "../include/exceptions/BadInput.h"

using namespace std;
using namespace model;
using namespace exceptions;

namespace util {

	/*
	 * Implementation of MapLoader utility class.
	 */

	const char MapLoader::SEP = ',';
	const char MapLoader::CSEP = ' ';

	typedef list<Location> LocationSet;
	typedef list<Reader> ReaderSet;
	typedef pair<string, unsigned int> NameIdPair;
	typedef unordered_map<string,unsigned int> NameIdMap;

	Map* MapLoader::loadMap(const string& mapFileName, const string& constrFileName) {
		ifstream mapFile(mapFileName);
		LocationSet locations;
		ReaderSet readers;
		NameIdMap locationsMap;
		string line;
		if (mapFile.is_open()) {
			unsigned int lId = 0, rId = 0;
			while (getline(mapFile, line)) {
				Util::trim(line);
				// Jump if blank line or comment (starts with '#')
				if (line.length() == 0 || line[0] == '#') continue;
				vector<string> v = Util::split(line, SEP);
				if (v.size() == 5) { // Location line
					string name = v[0];
					double x = stod(v[1]), y = stod(v[2]),
					       w = stod(v[3]), h = stod(v[4]);
					locationsMap.insert(NameIdPair(name, lId));
					locations.push_back(Location(lId++, name, x, y, w, h));
				} else if (v.size() == 3) { // Reader line
					string name = v[0];
					double x = stod(v[1]), y = stod(v[2]);
					readers.push_back(Reader(rId++, name, x, y));
				} else throw BadInput();
			}
		}
		Map* map = Map::createMap<LocationSet,ReaderSet>(locations, readers);
		ifstream constrFile(constrFileName);
		if (constrFile.is_open()) {
			while (getline(constrFile, line)) {
				Util::trim(line);
				// Jump if blank line or comment (starts with '#')
				if (line.length() == 0 || line[0] == '#') continue;
				vector<string> v = Util::split(line, CSEP);
				if (v[0] == "DR") { // Direct Reachability Constraint
					unsigned int id1 = locationsMap.at(v[1]), id2 = locationsMap.at(v[2]);
					map->setDR(id1, id2);
				} else if (v[0] == "DU") { // Direct Unreachability Constraint
					unsigned int id1 = locationsMap.at(v[1]), id2 = locationsMap.at(v[2]);
					map->setDU(id1, id2);
				} else if (v[0] == "LT") { // Latency Constraint
					unsigned int id = locationsMap.at(v[1]), latency = stoi(v[2]);
					map->setLT(id, latency);
				} else if (v[0] == "TT") { // Traveling Time Constraint
					unsigned int id1 = locationsMap.at(v[1]), id2 = locationsMap.at(v[2]),
						     travelingTime = stoi(v[3]);
					map->setTT(id1, id2, travelingTime);
				} else throw BadInput();
			}
		}
		return map;
	}

}
