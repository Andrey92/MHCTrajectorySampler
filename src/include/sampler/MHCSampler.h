#ifndef MHCSAMPLER_H
#define MHCSAMPLER_H

#include <stack>

#include "Types.h"
#include "../model/Map.h"
#include "../util/Types.h"
#include "Types.h"
#include "Probs.h"

namespace sampler {

	class MHCSampler {
		
		private:
			// Map object
			const Map* map;
			// Readings (collection of <{ri},t> pairs)
			const Readings* readings;
			// Set of likely locations for each time instant
			LocationRandomAccess** ls;
			// First sample generated with backtracking
			Trajectory* firstSample;
			// Collection of samples generated
			Samples samples;
			// Probabilities cache
			Probs p;

			// State variables for backtracking algorithm
			LocationRandomIterator* state;
			stack<unsigned long> prevIndexes;

			// Generates and returns first trajectory compatible with map and readings,
			// or nullptr if no valid trajectory is found.
			// Uses backtracking.
			virtual Trajectory* generateFirstSample(void);

			// Helper methods for backtracking
			virtual bool assignLocation(Trajectory*, unsigned long);
			virtual bool checkBackConstraints(Trajectory*, unsigned long&);
			virtual bool extendBackLatency(Trajectory*, unsigned long&, unsigned int, unsigned int);
			virtual bool isValidLocation(unsigned long, unsigned int);
			virtual bool assignNextLocation(Trajectory*, unsigned long);
			virtual void stepBack(Trajectory*, unsigned long&);
			virtual void resetLocations(Trajectory*, unsigned long);

			// Private method that really executes MHC,
			// used as base for public methods.
			Trajectory* generateSamples(Trajectory*, unsigned int, unsigned int, double);

			// Helper methods for sampling algorithm
			// Checks constraints (back and forward) from the specified position
			virtual bool checkConstraints(Trajectory*, unsigned long&);
			virtual bool extendLatency(Trajectory*, unsigned long&, unsigned int, unsigned int, bool);
			virtual bool checkForwardConstraints(Trajectory*, unsigned long, unsigned int, unsigned int);

			// Compute 'P(L|R)' probability for a particular time instant
			// relying on samples generated via MHC.
			virtual double computeProb(unsigned long, unsigned int) const;

		public:
			// Constructor
			MHCSampler(const Map*, const Readings*);

			// Returns sample (trajectory) size
			virtual unsigned long getSampleSize(void) const;

			// Generate trajectories using Metropolis Hastings with Constraints algorithm.
			// Accepts two positive integer: number of samples to produce,
			// and number of initial samples to discard (burn-in phase), and a perturbation factor,
			// which indicates the probability that a perturbation is applied at each time instant.
			// Each sample is a trajectory (succession of locations) which satisfy map constraints.
			virtual void generateSamples(unsigned long, unsigned long, double);

			// Same as above, but the first parameter indicates the dimension
			// of the "generation interval" (n). This function tries to generate 'n' more trajectories
			// only if in the previous 'n' trajectories there was at least one likelihood improvement.
			virtual unsigned long generateSamplesUntilWorth(unsigned long, unsigned long, double);

			// Given a particular time instant, returns the a-posterior probabilities,
			// as result of the Metropolis Hastings sampling algorithm.
			// Probabilities are of the form 'P(L|R)', which indicates the probability that
			// an object is in the location 'L' at the specified time,
			// given that it is detected by set 'R' of readers.
			virtual const ProbMap* getProbabilities(unsigned long) const;

	};

}

#endif
