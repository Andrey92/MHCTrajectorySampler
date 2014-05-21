#ifndef MHCSAMPLER_H
#define MHCSAMPLER_H

#include <stack>

#include "Types.h"
#include "../model/Map.h"
#include "../util/Types.h"

namespace sampler {

	class MHCSampler {
		
		private:
			// Map object
			const Map* map;
			// Readings (collection of <{ri},t> pairs)
			const Readings* readings;
			// Set of likely locations for each time instant
			LocationSet** ls;
			// Collection of samples generated
			Samples samples;

			// State variables for backtracking algorithm
			LocationIterator* state;
			stack<long> prevIndexes;

			// Helper methods for backtracking
			virtual bool assignLocation(Trajectory*, long);
			virtual bool checkConstraints(Trajectory*, long&);
			virtual bool extendLatency(Trajectory*, long&, unsigned int, unsigned int);
			virtual bool isValidLocation(long, unsigned int);
			virtual bool assignNextLocation(Trajectory*, long);
			virtual void stepBack(Trajectory*, long&);
			virtual void resetLocations(Trajectory* t, long);

		public:
			// Constructors
			MHCSampler(const Map*, const Readings*);

			// Generates and returns first trajectory compatible with map and readings,
			// or nullptr if no valid trajectory is found.
			// Uses backtracking.
			virtual Trajectory* generateFirstSample(void);

			// Generate trajectories using Metropolis Hastings with Constraints algorithm.
			// Accepts two positive integer: number of samples to produce,
			// and number of initial samples to discard (burn-in phase).
			// Each sample is a trajectory (succession of locations) which satisfy map constraints.
			virtual void generateSamples(unsigned long, unsigned long);

			// Given a particular time instant, returns the a-posterior probabilities,
			// as result of the Metropolis Hastings sampling algorithm.
			// Probabilities are of the form 'P(L|R)', which indicates the probability that
			// an object is in the location 'L' at the specified time,
			// given that it is detected by set 'R' of readers.
			virtual void printProbabilities(unsigned long) const;

	};

}

#endif
