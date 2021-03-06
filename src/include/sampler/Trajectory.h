#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <vector>

#include "../util/Identifiable.h"
#include "../util/HPFloat.h"

using namespace std;
using namespace util;

namespace sampler {

	/*
	 * A trajectory is a succession of locations, one for each time instant.
	 *
	 * For each location, it stores the likelihood probability,
	 * and computes the trajectory likelihood as the product of the single likelihoods.
	 */

	class Trajectory : public Identifiable {

		typedef vector<unsigned int> Locations;
		typedef vector<double> Likelihoods;
		typedef Locations::const_iterator loc_iterator;
		typedef Likelihoods::const_iterator like_iterator;

		private:
			// Trajectory id 
			unsigned int id;
			// Succession of locations (one for each time instant)
			Locations lo;
			// Likelihoods associated to locations in 'l'
			Likelihoods lh;
			// Trajectory likelihood (product of the likelihoods of each location)
			HPFloat likelihood;

			// Returns true iff argument is a valid time instant
			virtual bool isValidTI(unsigned long) const;
			// Returns true iff argument is a valid likelihood
			virtual bool isValidLH(double) const;

		public:
			// Constructor (receives number of time instants)
			Trajectory(unsigned int, unsigned long);
			Trajectory(unsigned int, const Trajectory&);
			// Destructor
			virtual ~Trajectory();

			// Getter methods
			virtual unsigned int getId(void) const;
			virtual unsigned long getSize(void) const;
			virtual unsigned int getLocation(unsigned long) const;
			virtual double getLikelihood(unsigned long) const;
			virtual const HPFloat& getTrajectoryLikelihood(void) const;

			// Set a location (specified by id) for a certain time instant, updating likelihood.
			virtual void setLocation(unsigned int, unsigned long, double);
			// Unset location for a certain time instant, updating likelihood.
			virtual void unsetLocation(unsigned long);

			// Equality operator
			virtual bool operator== (const Trajectory& t) const;

			// Iterator over locations
			virtual loc_iterator lbegin(void) const;
			virtual loc_iterator lend(void) const;

			// Iterator over likelihoods
			virtual like_iterator lhbegin(void) const;
			virtual like_iterator lhend(void) const;


	};

}

#endif
