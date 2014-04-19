#include <string>
#include <sstream>
#include <vector>

#include "../include/util/Util.h"

using namespace std;

namespace util {

	/*
	 * Implementation of Util class.
	 */

	const char Util::BLANKS[] = " \n\r\t";

	string& Util::trim(string& s) {
		s.erase(s.find_last_not_of(BLANKS) + 1);
		s.erase(0, s.find_first_not_of(BLANKS));
		return s;
	}

	vector<string> Util::split(const string& s, char delim) {
		stringstream ss(s);
		string tmp;
		vector<string> v;
		while (getline(ss, tmp, delim)) {
			if (trim(tmp) != "") v.push_back(tmp);
		}
		return v;
	}

	bool Util::comparePairs(pair<double, double> a, pair<double, double> b) {
		return a.second < b.second;
	}

}
