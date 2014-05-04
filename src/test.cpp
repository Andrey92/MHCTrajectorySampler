#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <list>

#include "include/model/Cell.h"
#include "include/model/Reader.h"
#include "include/model/Map.h"
#include "include/util/ConfLoader.h"
#include "include/util/MapLoader.h"
#include "include/exceptions/BadConf.h"
#include "include/exceptions/BadInput.h"

using namespace std;
using namespace model;
using namespace util;
using namespace exceptions;

void test(const Map&);

int main() {
	string confFile("conf/model.conf");
	cout << "Loading conf... " << flush;
	try {
		ConfLoader::loadConf(confFile);
	} catch(BadConf& e) {
		cout << e.what() << endl;
	} catch(exception& e) {
		cout << "Error while loading conf file" << endl;
	}
	cout << "Done!" << endl << endl;
	if (Reader::getModel() == ReaderModel::nstate) {
		cout << "Reader model loaded: n-state" << endl;
	}
	cout << "'Rate:radius' pairs (" << (Reader::nStates()) << " states): " << endl;
	for (auto it = Reader::begin(); it != Reader::end(); it++) {
		pair<double, double> rr = *it;
		cout << rr.first << ":" << rr.second << '\t';
	}
	cout << endl;
	cout << "Cell size: " << Cell::getSize() << "cm" << endl;
	string mapFile("input/map.txt");
	string constrFile("input/IC_DU_LT_TT.txt");
	cout << endl << "Loading map... " << flush;
	Map* m;
	try {
		m = MapLoader::loadMap(mapFile, constrFile);
	} catch(BadInput& e) {
		cout << e.what() << endl;
	} catch(exception& e) {
		cout << "Error while loading input files" << endl;
	}
	cout << "Done!" << endl << endl;
	test(*m);
	return 0;
}

void test(const Map& m) {
	cout << "==================== Map Info ====================" << endl << endl;
	cout << "Width: " << m.getWidth() << "cm | Height: " << m.getHeight() << "cm" << " | Cells: " << m.getCellCount() << endl;
	cout << endl << "----- Locations: " << m.getLocationCount() << " -----" << endl;
	for (auto it = m.lbegin(); it != m.lend(); it++) {
		Location l = *it;
		cout << "   Location: " << l.getName() << " | Latency: " << l.getLatency() << "s" <<
			" | Position: <" << l.getPos().getX() << ", " << l.getPos().getY() <<
			"> | Width: " << l.getWidth() << " | Height: " << l.getHeight() << endl;
	}
	cout << endl << "----- Readers: " << m.getReaderCount() << " -----" << endl;
	for (auto it = m.rbegin(); it != m.rend(); it++) {
		Reader r = *it;
		cout << "   Reader: " << r.getName() <<
			" | Position: <" << r.getPos().getX() << ", " << r.getPos().getY() << ">" << endl;
	}
	cout << endl << "----- Constraints -----" << endl;
	cout << "Direct Reachability matrix: " << endl;
	for (unsigned int i = 0; i < m.getLocationCount(); i++) {
		cout << "   ";
		for (unsigned int j = 0; j < m.getLocationCount(); j++) {
			cout << m.getDR(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl << "Latencies: " << endl << "   ";
	for (unsigned int i = 0; i < m.getLocationCount(); i++) {
		cout << m.getLT(i) << " ";
	}
	cout << endl << endl << "Traveling Time matrix: " << endl;
	for (unsigned int i = 0; i < m.getLocationCount(); i++) {
		cout << "   ";
		for (unsigned int j = 0; j < m.getLocationCount(); j++) {
			cout << setw(2) << m.getTT(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl << "----- Likely Locations test -----" << endl;
	for (auto it = m.rbegin(); it != m.rend(); it++) {
		Reader r = *it;
		cout << "Reader " << r.getName() << ": ";
		Map::LocationSet* ls = m.likelyLocations(r);
		for (auto l_it = ls->cbegin(); l_it != ls->cend(); l_it++) {
			Location l = *l_it;
			cout << l.getName() << " ";
		}
		cout << endl;
	}
	Reader r1 = *(m.rbegin() + 5);
	Reader r2 = *(m.rbegin());
	Reader r3 = *(m.rend() - 2);
	list<Reader> lr;
	lr.push_back(r1);
	lr.push_back(r2);
	lr.push_back(r3);
	cout << endl << "Set of reader ( ";
	for (auto it = lr.cbegin(); it != lr.cend(); it++) {
		cout << (*it).getName() << " ";
	}
	cout << ") : ";
	Map::LocationSet* ls = m.likelyLocations(lr);
	for (auto l_it = ls->cbegin(); l_it != ls->cend(); l_it++) {
		Location l = *l_it;
		cout << l.getName() << " ";
	}
	cout << endl << endl << "----- Likelihood test -----" << endl;
	Location l = *(ls->cbegin());
	double p = m.prob(lr, l);
	cout << "Probability that an object is detected by ( ";
	for (auto it = lr.cbegin(); it != lr.cend(); it++) {
		cout << (*it).getName() << " ";
	}
	cout << ") given that it is into " << l.getName() << ": " << p << endl;
	list<Reader> lr2;
	lr2.push_back(r2);
	p = m.prob(lr2, l);
	cout << "Probability that an object is detected by ( " << (*(lr2.cbegin())).getName() <<
		" ) given that it is into " << l.getName() << ": " << p << endl;
}
