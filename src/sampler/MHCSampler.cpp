#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

#include "../include/sampler/MHCSampler.h"

using namespace std;

namespace sampler {

	/*
	 * Implementation of MHCSampler.
	 *
	 * It generates first sample (trajectory) using backtracking.
	 */

	MHCSampler::MHCSampler(const Map* m, const Readings* r) : map(m), readings(r), p(m) {
		ls = new LocationSet*[r->size()];
		for (unsigned long l = 0; l < r->size(); l++) {
			ls[l] = map->likelyLocations(r->at(l).first);
		}
		firstSample = generateFirstSample();
		cout << "Backtracking trajectory: " << firstSample->getTrajectoryLikelihood().toString() << endl << endl;;
	}

	unsigned long MHCSampler::getSampleSize(void) const {
		return readings->size();
	}

	Trajectory* MHCSampler::generateFirstSample(void) {
		Trajectory* t = new Trajectory(0, readings->size());
		state = new LocationIterator[readings->size()];
		unsigned long l = 0;
		// Backtracking
		if (!assignLocation(t, l)) return nullptr;
		while (true) {
			if (checkBackConstraints(t, l)) {
				if (l == t->getSize() - 1) return t;
				if (assignLocation(t, l+1)) {
					l++;
					continue;
				}
			}
			while (l >= 0 && !assignNextLocation(t, l)) stepBack(t, l);
			if ((long)l == -1) return nullptr;
		}
		// Never reached
		return nullptr;
	}

	bool MHCSampler::assignLocation(Trajectory* t, unsigned long l) {
		resetLocations(t, l);
		return assignNextLocation(t, l);
	}

	bool MHCSampler::checkBackConstraints(Trajectory* t, unsigned long& l) {
		unsigned int id = t->getLocation(l);
		unsigned int lt = map->getLocation(id).getLatency();
		if (l == 0) {
			return extendBackLatency(t, l, id, lt);
		}
		unsigned long tmp = l - 1;
		unsigned int prev = t->getLocation(tmp);
		// Check Direct Reachability Constraint
		if (prev != id && !map->getDR(prev, id)) return false;
		
		// Check Traveling Time Constraints
		unsigned int back = map->getLocation(id).getMaxTimeFrom();
		while (back > 0) {
			unsigned long backPos = l - (unsigned long)back;
			if (backPos >= 0) {
				unsigned int backId = t->getLocation(backPos);
				if (map->getTT(backId, id) >= back) return false;
			}
			back--;
		}

		// Check latency constraint
		if (prev != id && !extendBackLatency(t, l, id, lt)) return false;
		return true;
	}

	bool MHCSampler::extendBackLatency(Trajectory* t, unsigned long& l, unsigned int id, unsigned int lt) {
		unsigned int tmplt = lt;
		unsigned long tmpl = l + 1;
		while (tmplt > 1) {
			if (tmpl >= t->getSize()) break;
			if (!isValidLocation(tmpl, id)) return false;
			tmplt--; tmpl++;
		}
		while (lt > 1) {
			if (l + 1 >= t->getSize()) return true;
			t->setLocation(id, l + 1, p.getProb(readings->at(l+1).first, id));
			lt--; l++;
		}
		return true;
	}

	bool MHCSampler::isValidLocation(unsigned long l, unsigned int id) {
		for (LocationIterator li = ls[l]->cbegin(); li != ls[l]->cend(); li++) {
			if (id == *li) return true;
		}
		return false;
	}

	bool MHCSampler::assignNextLocation(Trajectory* t, unsigned long l) {
		if (state[l] == ls[l]->cend()) {
			return false;
		}
		unsigned int id = *(state[l]);
		state[l]++;
		t->setLocation(id, l, p.getProb(readings->at(l).first, id));
		prevIndexes.push(l);
		return true;
	}

	void MHCSampler::stepBack(Trajectory* t, unsigned long& l) {
		unsigned long prevL = prevIndexes.top();
		prevIndexes.pop();
		while (l > prevL) {
			resetLocations(t, l);
			l--;
		}
	}

	void MHCSampler::resetLocations(Trajectory* t, unsigned long l) {
		state[l] = ls[l]->cbegin();
		t->unsetLocation(l);
	}

	bool MHCSampler::checkConstraints(Trajectory* t, unsigned long& l) {
		if (l == t->getSize() - 1) return checkBackConstraints(t, l);
		unsigned int id = t->getLocation(l);
		unsigned int lt = map->getLocation(id).getLatency();
		unsigned int next = t->getLocation(l + 1);
		if (l == 0) {
			if (id == next) return true;
			return extendLatency(t, l, id, lt, true);
		}
		unsigned int prev = t->getLocation(l - 1);

		// Check Direct Reachability Constraint
		if (prev != id && !map->getDR(prev, id)) return false;
		
		// Check prev latency constraint
		if (prev != id) {
			unsigned int prevLt = map->getLocation(prev).getLatency();
			for (unsigned long j = 1; j <= prevLt; j++) {
				if (j > l) return false;
				if (t->getLocation(l - j) != prev) return false;
			}
		}

		// Check Traveling Time Constraints
		unsigned int back = map->getLocation(id).getMaxTimeFrom();
		while (back > 0) {
			long backPos = l - (unsigned long)back;
			if (backPos >= 0) {
				unsigned int backId = t->getLocation((unsigned long)backPos);
				if (map->getTT(backId, id) >= back) return false;
			}
			back--;
		}

		// Check forward constraints (and latency)
		if (prev == id) {

			// Check next latency constraint
			if (next != id) {
				unsigned int nextLt = map->getLocation(next).getLatency();
				for (unsigned long j = 2; j <= nextLt; j++) {
					if (l + j >= t->getSize() || t->getLocation(l + j) != next)
						return extendLatency(t, l, id, j, false);
				}
			}

			// Check Direct Reachability and Traveling Time Constraints
			if (!checkForwardConstraints(t, l, id, next));

		} else if (!extendLatency(t, l, id, lt, true)) {
			return false;
		}
		return true;
	}

	bool MHCSampler::extendLatency(Trajectory* t, unsigned long& l, unsigned int id, unsigned int lt, bool checkNextLt) {
		unsigned int tmplt = lt;
		unsigned long tmpl = l + 1;
		while (tmplt > 1) {
			if (tmpl >= t->getSize()) break;
			if (!isValidLocation(tmpl, id)) return false;
			tmplt--; tmpl++;
		}
		if (tmpl < t->getSize()) {
			unsigned int next = t->getLocation(tmpl);
			if (checkNextLt) {
				if (next != id) {
					unsigned int nextLt = map->getLocation(next).getLatency();
					for (unsigned long j = 1; j < nextLt; j++) {
						if (tmpl + j >= t->getSize() || t->getLocation(tmpl + j) != next)
							return extendLatency(t, l, id, j + lt, false);
					}
				}
			}
			// Check Direct Reachability and Traveling Time Constraints
			if (!checkForwardConstraints(t, tmpl - 1, id, next)) return false;
		}
		while (lt > 1) {
			if (l + 1 >= t->getSize()) return true;
			t->setLocation(id, l + 1, p.getProb(readings->at(l+1).first, id));
			lt--; l++;
		}
		return true;
	}

	bool MHCSampler::checkForwardConstraints(Trajectory* t, unsigned long l, unsigned int id, unsigned int next) {
		// Check Direct Reachability Constraint
		if (next != id && !map->getDR(id, next)) return false;

		// Check Traveling Time Constraints
		unsigned long fw = (unsigned long)map->getLocation(id).getMaxTimeTo();
		while (fw > 0) {
			unsigned long fwPos = l + fw;
			if (fwPos < t->getSize()) {
				unsigned int fwId = t->getLocation(fwPos);
				if (map->getTT(id, fwId) >= fw) return false;
			}
			fw--;
		}
		return true;
	}

	void MHCSampler::generateSamples(unsigned long n, unsigned long b) {
		generateSamples(firstSample, n, b);
	}

	void MHCSampler::generateSamplesUntilWorth(unsigned long n, unsigned long b) {
		Trajectory* t = firstSample;
		unsigned long i = 0;
		do {
			cout << endl << "[Iteration " << i << "] Try to generate " << n << " trajectories:" << endl << endl;
			t = generateSamples(t, n, b);
			i++;
			b = 0; // Burn-in phase only on first iteration
		} while (t != nullptr);
		cout << endl << "No likelihood improvements in last " << n << " trajectories: MHC stopped." << endl;
	}

	Trajectory* MHCSampler::generateSamples(Trajectory* t, unsigned int n, unsigned int b) {
		srand(time(NULL));
		bool improvement = false;
		bool burnIn = true;
		for (unsigned long i = 0; i < n + b; i++) {
			Trajectory* tNew = new Trajectory(i + 1, *t);
			for (unsigned long j = 0; j < readings->size(); j++) {
				// Choose a random location for instant 'j' (among likely locations)
				LocationSet* l = ls[j];
				long count = 0;
				do {
					unsigned int pos = (unsigned int) rand() % l->size();
					auto it = l->cbegin();
					for ( ; pos > 0; it++, pos--) ;
					unsigned int id = *it;
					tNew->setLocation(id, j, p.getProb(readings->at(j).first, id));
					if (count > 1000) cout << j << " " << flush;
					count++;
				} while (!checkConstraints(tNew, j));
			}
			// Metropolis Hastings
			long double jitter = (long double) rand() / RAND_MAX;
			HPFloat w = tNew->getTrajectoryLikelihood() / t->getTrajectoryLikelihood();
			if (jitter < w.getValue()) {
				cout << "[MHC at iteration " << setw(8) << i << "] | Lnew: " << setw(18) << tNew->getTrajectoryLikelihood().toString() <<
					" | Lnew/L: " << setw(18) << w.toString() << " | Jitter: " << jitter << endl;
				delete t;
				t = tNew;
				improvement = true;
			} else {
				delete tNew;
			}
			if (burnIn && i >= b) {
				if (b > 0) cout << "[End of the burn-in phase]" << endl;
				burnIn = false;
			}
			if (i >= b) {
				Samples::iterator it;
				if ((it = samples.find(*t)) == samples.end()) {
					samples.insert(MultiSample(*t, 1));
				} else {
					it->second++;
				}
			}
		}
		return improvement ? t : nullptr;
	}

	const ProbMap* MHCSampler::getProbabilities(unsigned long t) const {
		ProbMap* pm = new ProbMap();
		for (auto it = samples.cbegin(); it != samples.cend(); it++) {
			unsigned int id = it->first.getLocation(t);
			if (pm->find(id) == pm->end()) {
				pm->insert(LRProb(id, computeProb(t, id)));
			}
		}
		return pm;
	}

	double MHCSampler::computeProb(unsigned long t, unsigned int id) const {
		unsigned long count = 0;
		unsigned long size = 0;
		for (auto it = samples.cbegin(); it != samples.cend(); it++) {
			size += it->second;
			if (it->first.getLocation(t) == id) count += it->second;
		}
		return count / (double) size;
	}

}
