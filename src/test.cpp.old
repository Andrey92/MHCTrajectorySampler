#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <list>
#include <stdexcept>

#include "include/model/Cell.h"
#include "include/model/Reader.h"
#include "include/model/Map.h"
#include "include/sampler/MHCSampler.h"
#include "include/util/ConfLoader.h"
#include "include/util/MapLoader.h"
#include "include/util/ReadingsLoader.h"
#include "include/exceptions/BadConf.h"
#include "include/exceptions/BadInput.h"

using namespace std;
using namespace model;
using namespace sampler;
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
	cout << "Done!" << endl;
/*	cout << endl;
	if (Reader::getModel() == ReaderModel::nstate) {
		cout << "Reader model loaded: n-state" << endl;
	}
	cout << "'Rate:radius' pairs (" << (Reader::nStates()) << " states): " << endl;
	for (auto it = Reader::begin(); it != Reader::end(); it++) {
		pair<double, double> rr = *it;
		cout << rr.first << ":" << rr.second << '\t';
	}
	cout << endl;
	cout << "Cell size: " << Cell::getSize() << "cm" << endl << endl;*/
	string mapFile("input/map.txt");
	string constrFile("input/IC_DU_LT_TT.txt");
	cout << "Loading map... " << flush;
	Map* m;
	try {
		m = MapLoader::loadMap(mapFile, constrFile);
	} catch(BadInput& e) {
		cout << e.what() << endl;
	} catch(exception& e) {
		cout << "Error while loading input files" << endl;
	}
	cout << "Done!" << endl << endl;
	//test(*m);
	string readingsFile("input/readings10min.txt");
	Readings* r;
	try {
		r = ReadingsLoader::loadReadings(readingsFile, *m);
	} catch(BadInput& e) {
		cout << e.what() << endl;
	} catch(exception& e) {
		cout << "Error while loading input files" << endl;
	}
/*	list<unsigned int> l1;
	list<unsigned int> l2;
	list<unsigned int> l3;
	list<unsigned int> l4;
	
	l2.push_back(2);
	
	l3.push_back(1);

	l4.push_back(2);
	l4.push_back(1);
	
	double p1 = m->prob(l1, 9);
	double p2 = m->prob(l2, 9);
	double p3 = m->prob(l3, 9);
	double p4 = m->prob(l4, 9);
	cout << p1 << " + " << p2 << " + " << p3 << " + " << p4 << " = " << p1 + p2 + p3 + p4 << endl;*/
/*	for (auto it = r->cbegin(); it != r->cend(); it++) {
		cout << "{ ";
		for (auto r_it = it->first.cbegin(); r_it != it->first.cend(); r_it++) {
			cout << m->getReader(*r_it).getName() << " ";
		}
		cout << "} : " << it->second << endl;
	}*/
	MHCSampler mhcs(m, r);
	mhcs.generateSamplesUntilWorth(10000, 0);
	for (unsigned long time = 0; time < mhcs.getSampleSize(); time++) {
		const ProbMap* pm = mhcs.getProbabilities(time);
		cout << "Probabilities at instant " << time*500 << ":" << endl;
		for (auto it = pm->cbegin(); it != pm->cend(); it++) {
			cout << "    P(" << m->getLocation(it->first).getName() << ") = " << it->second << endl;
		}
		cout << endl;
	}
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
		LocationSet* ls = m.likelyLocations(r.getId());
		for (auto l_it = ls->cbegin(); l_it != ls->cend(); l_it++) {
			unsigned int l = *l_it;
			cout << m.getLocation(l).getName() << " ";
		}
		cout << endl;
	}
	Reader r1 = *(m.rbegin() + 5);
	Reader r2 = *(m.rbegin());
	Reader r3 = *(m.rend() - 2);
	list<unsigned int> lr;
	lr.push_back(r1.getId());
	lr.push_back(r2.getId());
	lr.push_back(r3.getId());
	cout << endl << "Set of reader ( ";
	for (auto it = lr.cbegin(); it != lr.cend(); it++) {
		cout << m.getReader(*it).getName() << " ";
	}
	cout << ") : ";
	LocationSet* ls = m.likelyLocations(lr);
	for (auto l_it = ls->cbegin(); l_it != ls->cend(); l_it++) {
		unsigned int l = *l_it;
		cout << m.getLocation(l).getName() << " ";
	}
	cout << endl << endl << "----- Likelihood test -----" << endl;
	Location l = *(ls->cbegin());
	double p = m.prob(lr, l.getId());
	cout << "Probability that an object is detected by ( ";
	for (auto it = lr.cbegin(); it != lr.cend(); it++) {
		cout << m.getReader(*it).getName() << " ";
	}
	cout << ") given that it is into " << l.getName() << ": " << p << endl;
	list<unsigned int> lr2;
	lr2.push_back(r2.getId());
	p = m.prob(lr2, l.getId());
	cout << "Probability that an object is detected by ( " << r2.getName() <<
		" ) given that it is into " << l.getName() << ": " << p << endl;
}
