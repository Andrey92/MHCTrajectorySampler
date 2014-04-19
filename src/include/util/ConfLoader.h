#ifndef CONF_LOADER_H
#define CONF_LOADER_H

#include <string>
#include <vector>

using namespace std;

namespace util {

	/*
	 * ConfLoader handles configuration file parsing.
	 */

	class ConfLoader {

		private:
			// Private constructor (static class)
			ConfLoader() { };

		public:
			// Characters used in conf file:
			// To assign values
			static const char ASSIGNMENT;
			// To separate values
			static const char SEP;
			// To specify two or more sub-values for each value
			static const char SUBSEP;

			// Load configuration from specified file name
			static void loadConf(const string&);

	};

}

#endif
