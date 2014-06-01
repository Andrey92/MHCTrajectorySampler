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

#define MIN_NUM_SAMPLES 1000
#define MIN_BURN_IN_PHASE 0

using namespace std;
using namespace model;
using namespace sampler;
using namespace util;
using namespace exceptions;

// Helper functions
void loadParams(int, char**);
void help(char*);
void loadConf(void);
Map* loadMap(void);
Readings* loadReadings(const Map&);
const vector<double>& getMHCAccuracy(const MHCSampler&, const Map&);

// MHC parameters:
// Until worth mode, number of samples, size of the burn-in phase and perturbation factor.
bool untilWorth = false;
unsigned long n = -1, b = -1;
double f = -1;

int main(int argc, char** argv) {
	// Load MHC params
	loadParams(argc, argv);

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
	unsigned long iterations = 1;
	if (untilWorth) {
		iterations = mhcs.generateSamplesUntilWorth(n, b, f);
	} else {
		mhcs.generateSamples(n, b, f);
	}
	end = clock();
	cout << endl << "Time elapsed to generate " << n * iterations << " trajectories: " << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl << endl;
	const vector<double> v = getMHCAccuracy(mhcs, *m);
	cout << endl << "Results of MHCSampler with " << n - b + n * (iterations - 1) << " accepted samples and a perturbation factor of " << f << ":" << endl <<
		"   Accuracy: " << v[0] << endl << "   Location miss rate: " << v[1] << endl;
	return 0;
}

void loadParams(int argc, char** argv) {
	if (argc == 2 || argc == 5) {
		string wOption(argv[1]);
		if (wOption != "-w") {
			help(argv[0]);
			exit(-1);
		} else {
			untilWorth = true;
		}
	}
	if (argc == 1 || argc == 2) {
		// Load parameters from stdin
		do {
			try {
				cout << "Specify number of samples to generate: ";
				string line;
				if (!getline(cin, line)) {
					cout << endl;
					exit(-1);
				}
				n = stoul(line);
				if ((long)n < 0 || n < MIN_NUM_SAMPLES) throw "Value must be >= " + to_string(MIN_NUM_SAMPLES);
			} catch(const string& s) {
				cout << s << endl;
			} catch(exception& e) {
				cout << "Insert an integer value!" << endl;
			}
		} while ((long)n < 0);

		do {
			try {
				cout << "Specify number of samples of the burn-in phase: ";
				string line;
				if (!getline(cin, line)) {
					cout << endl;
					exit(-1);
				}
				b = stoul(line);
				if ((long)b < 0 || b < MIN_BURN_IN_PHASE) throw "Value must be >= " + to_string(MIN_BURN_IN_PHASE);
			} catch(const string& s) {
				cout << s << endl;
			} catch(exception& e) {
				cout << "Insert an integer value!" << endl;
			}
		} while ((long)b < 0);

		do {
			try {
				cout << "Choose perturbation factor from range [0,1]: ";
				string line;
				if (!getline(cin, line)) {
					cout << endl;
					exit(-1);
				}
				f = stod(line);
				if (f < 0 || f > 1) throw "Value must be in range [0,1]";
			} catch(const string& s) {
				cout << s << endl;
			} catch(exception& e) {
				cout << "Insert a decimal number!" << endl;
			}
		} while (f < 0);

	} else if (argc == 4 || argc == 5) {
		int offset = argc - 4;
		try {
			n = stoul(argv[1 + offset]);
			if ((long)n < 0 || n < MIN_NUM_SAMPLES) throw "Value must be >= " + to_string(MIN_NUM_SAMPLES);
			b = stoul(argv[2 + offset]);
			if ((long)b < 0 || b < MIN_BURN_IN_PHASE) throw "Value must be >= " + to_string(MIN_BURN_IN_PHASE);
			f = stod(argv[3 + offset]);
			if (f < 0 || f > 1) throw "Value must be in range [0,1]";
		} catch(const string& s) {
			cout << s << endl;
			exit(-1);
		} catch(exception& e) {
			cout << "Invalid arguments given." << endl;
			help(argv[0]);
			exit(-1);
		}
	} else {
		help(argv[0]);
		exit(-1);
	}
}

void help(char* argv0) {
	cout << "Usage: " << argv0 << " [-w] [<num_samples> <burn_in_size> <perturbation_factor>]" << endl << endl <<
		"OPTIONS" << endl <<
		"  -w: enables 'until worth' mode (disabled by default), generates a new set of samples until there is at least an accepted sample in the last iteration" << endl << endl <<
		"ARGUMENTS" << endl <<
		"  <num_samples>: the number of samples to generate" << endl <<
		"  <burn_in_size>: the number of samples to discard during the burn-in (initial) phase" << endl <<
		"  <perturbation_factor>: the probability that a perturbation is applied for each time instant" << endl;
}

void loadConf(void) {
	string confFile(CONF_FILE);
	cout << "Loading conf... " << flush;
	try {
		ConfLoader::loadConf(confFile);
	} catch(BadConf& e) {
		cout << e.what() << endl;
		exit(-2);
	} catch(exception& e) {
		cout << "Error while loading conf file" << endl;
		exit(-2);
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
		exit(-3);
	} catch(exception& e) {
		cout << "Error while loading input files" << endl;
		exit(-3);
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
			delete realTrajectory;
			realTrajectory = nullptr;
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
