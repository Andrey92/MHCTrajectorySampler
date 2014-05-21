#include <cstdlib>
#include <ctime>

#include "../include/sampler/MHCSampler.h"

using namespace std;

namespace sampler {

	/*
	 * Implementation of MHCSampler.
	 *
	 * It generates first sample (trajectory) using backtracking.
	 */

	MHCSampler::MHCSampler(const Map* m, const Readings* r) {
		this->map = m;
		this->readings = r;
		ls = new LocationSet*[r->size()];
		for (unsigned long l = 0; l < r->size(); l++) {
			ls[l] = map->likelyLocations(r->at(l).first);
		}
	}

	Trajectory* MHCSampler::generateFirstSample(void) {
		Trajectory* t = new Trajectory(readings->size());
		state = new LocationIterator[readings->size()];
		long l = 0;
		// Backtracking
		if (!assignLocation(t, l)) return nullptr;
		while (true) {
			if (checkConstraints(t, l)) {
				if (l == (long)t->getSize() - 1) return t;
				if (assignLocation(t, l+1)) {
					l++;
					continue;
				}
			}
			while (l >= 0 && !assignNextLocation(t, l)) stepBack(t, l);
			if (l == -1) return nullptr;
		}
		// Never reached
		return nullptr;
	}

	bool MHCSampler::assignLocation(Trajectory* t, long l) {
		resetLocations(t, l);
		return assignNextLocation(t, l);
	}

	bool MHCSampler::checkConstraints(Trajectory* t, long& l) {
		unsigned int id = t->getLocation(l);
		unsigned int lt = map->getLocation(id).getLatency();
		if (l == 0) {
			return extendLatency(t, l, id, lt);
		}
		long tmp = l - 1;
		unsigned int prev = t->getLocation(tmp);
		// Check Direct Reachability Constraint
		if (prev != id && !map->getDR(prev, id)) return false;
		
		// Check Traveling Time Constraints
		unsigned int back = map->getLocation(id).getMaxTimeFrom();
		while (back > 0) {
			long backPos = l - (long)back;
			if (backPos >= 0) {
				unsigned int backId = t->getLocation(backPos);
				if (map->getTT(backId, id) >= back) return false;
			}
			back--;
		}

		// Check latency constraint
		if (prev != id && !extendLatency(t, l, id, lt)) return false;
		return true;
	}

	bool MHCSampler::extendLatency(Trajectory* t, long& l, unsigned int id, unsigned int lt) {
		unsigned int tmplt = lt;
		long tmpl = l + 1;
		while (tmplt > 1) {
			if (!isValidLocation(tmpl, id)) return false;
			tmplt--; tmpl++;
		}
		while (lt > 1) {
			t->setLocation(id, l+1, /* TODO clean */ map->prob(readings->at(l+1).first, id));
			lt--; l++;
		}
		return true;
	}

	bool MHCSampler::isValidLocation(long l, unsigned int id) {
		for (LocationIterator li = ls[l]->cbegin(); li != ls[l]->cend(); li++) {
			if (id == *li) return true;
		}
		return false;
	}

	bool MHCSampler::assignNextLocation(Trajectory* t, long l) {
		if (state[l] == ls[l]->cend()) {
			return false;
		}
		unsigned int id = *(state[l]);
		state[l]++;
		t->setLocation(id, l, /* TODO clean */ map->prob(readings->at(l).first, id));
		prevIndexes.push(l);
		return true;
	}

	void MHCSampler::stepBack(Trajectory* t, long& l) {
		long prevL = prevIndexes.top();
		prevIndexes.pop();
		while (l > prevL) {
			resetLocations(t, l);
			l--;
		}
	}

	void MHCSampler::resetLocations(Trajectory* t, long l) {
		state[l] = ls[l]->cbegin();
		t->unsetLocation(l);
	}

	void MHCSampler::generateSamples(unsigned long n, unsigned long b) {
		Trajectory* t = generateFirstSample();
		srand(time(NULL));
		for (unsigned long i = 0; i < n + b; i++) {
			Trajectory* tNew = new Trajectory(*t);
			for (unsigned long j = 0; j < readings->size(); j++) {
				// Choose a random location for instant 'j' (among likely locations)
				LocationSet* l = ls[j];
				long lj = (long)j;
				while (!checkConstraints(tNew, lj)) { // WARNING: Possible infinity loop
					unsigned int pos = (unsigned int) rand() % l->size();
					auto it = l->cbegin();
					for ( ; pos > 0; it++, pos--) ;
					unsigned int id = *it;
					t->setLocation(id, j, /* TODO clean */ map->prob(readings->at(j).first, id));
				}
				j = (unsigned long)lj;
			}
			// Metropolis Hastings
			long double jitter = (long double) rand() / RAND_MAX;
			HPFloat w = tNew->getTrajectoryLikelihood() / t->getTrajectoryLikelihood();
			if (jitter < w.getValue()) {
				delete t;
				t = tNew;
				if (i >= b) {
					samples.push_back(*t);
				}
			}
		}
	}

	void MHCSampler::printProbabilities(unsigned long t) const {
		// TODO
	}

}
