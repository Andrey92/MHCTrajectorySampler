#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>
#include <list>

#include "../model/Map.h"

using namespace std;
using namespace model;

namespace util {

	/*
	 * MapLoader handles map file parsing.
	 *
	 * It loads locations, readers and map constraints.
	 */

	class MapLoader {

		typedef list<Location> LoadableLocationSet;
		typedef list<Reader> LoadableReaderSet;

		private:
			// Private constructor (static class)
			MapLoader() { };

		public:
			// Characters used in map file:
			// To separate values (locations and readers)
			static const char SEP;
			// To separate values (constraints)
			static const char CSEP;

			// Load map informations from specified file names (map and constraints)
			static Map* loadMap(const string&, const string&);

	};

}

#endif
