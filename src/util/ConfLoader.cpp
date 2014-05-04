#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

#include "../include/model/Reader.h"
#include "../include/model/Cell.h"
#include "../include/util/ConfLoader.h"
#include "../include/util/Util.h"
#include "../include/exceptions/BadConf.h"

using namespace std;
using namespace model;
using namespace exceptions;

namespace util {

	/*
	 * Implementation of ConfLoader utility class.
	 */

	const char ConfLoader::ASSIGNMENT = '=';
	const char ConfLoader::SEP = ',';
	const char ConfLoader::SUBSEP = ':';

	void ConfLoader::loadConf(const string& file) {
		ifstream confFile(file);
		if (confFile.is_open()) {
			string line;
			while (getline(confFile, line)) {
				Util::trim(line);
				// Jump if blank line or comment (starts with '#')
				if (line.length() == 0 || line[0] == '#') continue;
				vector<string> v = Util::split(line, ASSIGNMENT);
				if (v.size() != 2) throw BadConf();
				if (v[0] == "rmodel") {
					if (v[1] == "n-state") {
						Reader::rModel = ReaderModel::nstate;
					} else throw BadConf();
				} else if (v[0] == "nstateparams") {
					vector<string> values = Util::split(v[1], SEP);
					if (values.size() < 1) throw BadConf();
					vector<pair<double, double>> states;
					for (auto it = values.begin(); it != values.end(); it++) {
						vector<string> subval = Util::split(*it, SUBSEP);
						if (subval.size() != 2) throw BadConf();
						pair<double, double> rate_radius(stod(subval[0]), stod(subval[1]));
						states.push_back(rate_radius);
					}
					sort(states.begin(), states.end(), Util::comparePairs);
					double inf = numeric_limits<double>::infinity();
					if (states[states.size() - 1].second != inf) {
						states.push_back(pair<double, double>(0.0, inf));
					}
					Reader::states = states;
				} else if (v[0] == "cellsize") {
					Cell::size = stod(v[1]);
				} else throw BadConf();
			}
			confFile.close();
		} else {
			cout << "Unable to read conf file" << endl;
		}
	}

}
