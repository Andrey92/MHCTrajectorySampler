#include <string>
#include <vector>
#include <utility>

#include "../include/model/Reader.h"

using namespace std;

namespace model {

	/*
	 * Implementation of Reader class.
	 */

	// Definition of static variables whose values depend on conf file.
	ReaderModel Reader::rModel;
	vector<pair<double, double>> Reader::states;

	ReaderModel Reader::getModel(void) {
		return rModel;
	}

	unsigned int Reader::nStates(void) {
		return states.size();
	}

	unsigned int Reader::getId(void) const {
		return id;
	}

	const string& Reader::getName(void) const {
		return name;
	}

	const Point& Reader::getPos(void) const {
		return position;
	}

	double Reader::getRate(const Point& p) const {
		double rate = 0.0;
		if (rModel == ReaderModel::nstate) {
			double dist = position.distance(p);
			for (auto it = states.begin(); it != states.end(); it++) {
				pair<double, double> cur = *it;
				if (dist < cur.second) {
					rate = cur.first;
					break;
				}
			}
		}
		return rate;
	}

	vector<pair<double, double>>::iterator Reader::begin(void) {
		return states.begin();
	}
	
	vector<pair<double, double>>::iterator Reader::end(void) {
		return states.end();
	}

}