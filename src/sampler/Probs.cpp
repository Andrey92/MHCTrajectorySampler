#include "../include/sampler/Probs.h"

namespace sampler {

	/*
	 * Implementation of Probs utility class
	 */

	Probs::Probs(const Map* m) : m(m) { }

	double Probs::getProb(const ReaderSet& rs, unsigned int id) {
		RL rl(rs, id);
		LikeliMap::const_iterator it;
		if ((it = lm.find(rl)) == lm.end()) {
			double p = m->prob(rs, id);
			lm.insert(RLProb(rl, p));
			return p;
		}
		return it->second;
	}

}
