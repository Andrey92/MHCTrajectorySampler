#include <iostream>
#include <string>
#include <utility>

#include "include/model/Cell.h"
#include "include/model/Reader.h"
#include "include/util/ConfLoader.h"
#include "include/exceptions/BadConf.h"

using namespace std;
using namespace model;
using namespace util;
using namespace exceptions;

int main() {
	string confFile("conf/model.conf");
	try {
		ConfLoader::loadConf(confFile);
	} catch(BadConf& e) {
		cout << e.what() << endl;
	} catch(exception& e) {
		cout << "Error while loading conf file" << endl;
	}
	if (Reader::getModel() == ReaderModel::nstate) {
		cout << "Reader model loaded: n-state" << endl;
	}
	cout << "'Rate:radius' pairs (" << (Reader::nStates()) << " states): " << endl;
	for (auto it = Reader::begin(); it != Reader::end(); it++) {
		pair<double, double> rr = *it;
		cout << rr.first << ":" << rr.second << '\t';
	}
	cout << endl;
	cout << "Cell size (meters): " << Cell::getSize() << endl;
	return 0;
}
