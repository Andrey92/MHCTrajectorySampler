#include <stdexcept>

#include "../include/sampler/Trajectory.h"

namespace sampler {

	Trajectory::Trajectory(unsigned long size) : lo(size), lh(size), likelihood(1.0) {
		for (unsigned long i = 0; i < size; i++) {
			lo[i] = -1;
			lh[i] = 1.0;
		}
	}

	Trajectory::Trajectory(const Trajectory& t) : lo(t.lo), lh(t.lh), likelihood(t.likelihood) { }

	Trajectory::~Trajectory() { }

	bool Trajectory::isValidTI(unsigned long t) const {
		return t >= 0 && t < lo.size();
	}

	bool Trajectory::isValidLH(double lh) const {
		return lh >= 0.0 && lh <= 1.0;
	}

	unsigned long Trajectory::getSize(void) const {
		return lo.size();
	}

	unsigned int Trajectory::getLocation(unsigned long t) const {
		 if (!isValidTI(t)) throw invalid_argument("Invalid time instant");
		 return lo[t];
	}

	double Trajectory::getLikelihood(unsigned long t) const {
		 if (!isValidTI(t)) throw invalid_argument("Invalid time instant");
		 return lh[t];
	}

	const HPFloat& Trajectory::getTrajectoryLikelihood(void) const {
		 return likelihood;
	}

	void Trajectory::setLocation(unsigned int id, unsigned long t, double l) {
		if (!isValidTI(t)) throw invalid_argument("Invalid time instant");
		if (!isValidLH(l)) throw invalid_argument("Invalid likelihood");
		lo[t] = id;
		likelihood.div(lh[t]);
		lh[t] = l;
		likelihood.mul(lh[t]);
	}

	void Trajectory::unsetLocation(unsigned long t) {
		if (!isValidTI(t)) throw invalid_argument("Invalid time instant");
		lo[t] = -1;
		likelihood.div(lh[t]);
		lh[t] = 1.0;
		likelihood.mul(lh[t]);
	}

	Trajectory::loc_iterator Trajectory::lbegin(void) const {
		return lo.cbegin();
	}

	Trajectory::loc_iterator Trajectory::lend(void) const {
		return lo.cend();
	}

	Trajectory::like_iterator Trajectory::lhbegin(void) const {
		return lh.cbegin();
	}

	Trajectory::like_iterator Trajectory::lhend(void) const {
		return lh.cend();
	}

}
