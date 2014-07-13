#ifndef ADJACENTS_H
#define ADJACENTS_H

#include "Map.h"
#include "../util/Types.h"

namespace model {

	/*
	 * Contains arrays of adjacents for each location
	 */

	class Adjacents {

		private:
			// Adjacents arrays
			LocationRandomAccess** ad;
			// Number of adjacents for each location
			unsigned int* n_ad;

		public:
			// Constructor
			Adjacents(const Map* m);

			// Returns the number of adjacents of the location identified by id
			unsigned int getAdjacentsCount(unsigned int) const;

			// Returns the pointer to the array of adjacents of the location identified by the parameter
			const LocationRandomAccess* getAdjacents(unsigned int) const;

	};

}

#endif
