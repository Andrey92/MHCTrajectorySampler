#ifndef PROBS_H
#define PROBS_H

#include "../model/Map.h"
#include "Types.h"

namespace sampler {
	
	/*
	 * Probs class performs caching of 'P(R|L)' values using a map container.
	 *
	 * It applies 'Proxy' design pattern.
	 */

	class Probs {

		private:
			// Topological map object.
			const Map* m;
			// Map of the probability values 'P(R|L)'.
			// Used as values cache.
			LikeliMap lm;

		public:
			// Minimum probability value
			// Each location with a probability < MIN_PROB should be discarded.
			static const double MIN_PROB;

			// Constructor
			Probs(const Map*);

			// Getter method for a probability value
			virtual double getProb(const ReaderSet&, unsigned int);

	};

}

#endif
