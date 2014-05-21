#ifndef READINGS_LOADER_H
#define READINGS_LOADER_H

#include <string>

#include "Types.h"
#include "../model/Map.h"

using namespace std;
using namespace model;

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

	class ReadingsLoader {
		
		private:
			// Private constructor (static class)
			ReadingsLoader() { };

		public:

			// Characters used in map file:
			// To separate values (locations and readers)
			static const char SEP;
			// To delimit reader set
			static const char STARTSET;
			static const char ENDSET;

			// Load readings from specified file name
			// It uses a Map object to identify Readers
			static Readings* loadReadings(const string&, const Map&);
	};

}

#endif
