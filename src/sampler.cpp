#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <list>
#include <stdexcept>
#include <ctime>

#include "include/model/Cell.h"
#include "include/model/Reader.h"
#include "include/model/Map.h"
#include "include/sampler/MHCSampler.h"
#include "include/util/ConfLoader.h"
#include "include/util/MapLoader.h"
#include "include/util/ReadingsLoader.h"
#include "include/util/TrajectoryLoader.h"
#include "include/exceptions/BadConf.h"
#include "include/exceptions/BadInput.h"

#define CONF_FILE "conf/model.conf"
#define MAP_FILE "input/map.txt"
#define CONSTRAINTS_FILE "input/IC_DU_LT_TT.txt"
#define READINGS_FILE "input/010-readings.txt"
#define TRAJECTORY_FILE "input/010-trajectory.txt"

#define N 50000
#define B 500

using namespace std;
using namespace model;
using namespace sampler;
using namespace util;
using namespace exceptions;

void loadConf(void);
Map* loadMap(void);
Readings* loadReadings(const Map&);
const vector<double>& getMHCAccuracy(const MHCSampler&, const Map&);

int main(int argc, char** argv) {
	// Load configuration file
	loadConf();

	// Load map file
	Map* m = loadMap();

	// Load readings input file (specified from user)
	Readings* r = loadReadings(*m);

	// Instantiate the sampler
	MHCSampler mhcs(m, r);

	// Run MHCSampler algorithm,
	// measuring efficiency and accuracy.
	clock_t start, end;
	start = clock();
	mhcs.generateSamples(N, B);
	end = clock();
	cout << endl << "Time elapsed to generate " << N << " trajectories: " << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl;
	const vector<double> v = getMHCAccuracy(mhcs, *m);
	cout << endl << "Accuracy of MHCSampler with " << N - B << " samples: " << v[0] << endl <<
		"Location miss rate: " << v[1] << endl;
	return 0;
}

void loadConf(void) {
	string confFile(CONF_FILE);
	cout << "Loading conf... " << flush;
	try {
		ConfLoader::loadConf(confFile);
	} catch(BadConf& e) {
		cout << e.what() << endl;
		exit(-1);
	} catch(exception& e) {
		cout << "Error while loading conf file" << endl;
		exit(-1);
	}
	cout << "Done!" << endl;
}

Map* loadMap(void) {
	string mapFile(MAP_FILE);
	string constrFile(CONSTRAINTS_FILE);
	cout << "Loading map... " << flush;
	Map* m = nullptr;
	try {
		m = MapLoader::loadMap(mapFile, constrFile);
	} catch(BadInput& e) {
		cout << e.what() << endl;
		exit(-1);
	} catch(exception& e) {
		cout << "Error while loading input files" << endl;
		exit(-1);
	}
	cout << "Done!" << endl << endl;
	return m;
}

Readings* loadReadings(const Map& m) {
	Readings* r = nullptr;
	do {
		cout << "Specify readings file: ";
		string readingsFile;
		getline(cin, readingsFile);
		try {
			r = ReadingsLoader::loadReadings(readingsFile, m);
		} catch(BadInput& e) {
			cout << e.what() << endl;
		} catch(exception& e) {
			cout << "Error while loading input files" << endl;
		}
	} while (r == nullptr);
	return r;
}

const vector<double>& getMHCAccuracy(const MHCSampler& mhcs, const Map& m) {
	Trajectory* realTrajectory = nullptr;
	do {
		cout << "Specify trajectory file: ";
		string trajectoryFile;
		getline(cin, trajectoryFile);
		try {
			realTrajectory = TrajectoryLoader::loadTrajectory(trajectoryFile, m);
			if (realTrajectory->getSize() != mhcs.getSampleSize()) throw BadInput();
		} catch(BadInput& e) {
			cout << e.what() << endl;
		} catch(exception& e) {
			cout << "Error while loading input files" << endl;
		}
	} while (realTrajectory == nullptr);
	double acc = 1.0;
	double miss = 0.0;
	for (unsigned long time = 0; time < mhcs.getSampleSize(); time++) {
		const ProbMap* pm = mhcs.getProbabilities(time);
		double tmp = acc;
		for (auto it = pm->cbegin(); it != pm->cend(); it++) {
			if (it->first == realTrajectory->getLocation(time)) {
				acc += it->second;
				break;
			}
		}
		if (acc == tmp) miss++;
	}
	acc /= (double)mhcs.getSampleSize();
	miss /= (double)mhcs.getSampleSize();
	vector<double>* v = new vector<double>();
	v->push_back(acc);
	v->push_back(miss);
	return *v;
}
