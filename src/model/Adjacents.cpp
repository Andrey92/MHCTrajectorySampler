#include "../include/model/Adjacents.h"

namespace model {

	/*
	 * Implementation of Adjacents class.
	 */

	Adjacents::Adjacents(const Map* m) {
		unsigned int locCount = m->getLocationCount();
		ad = new LocationRandomAccess*[locCount];
		n_ad = new unsigned int[locCount];
		for (unsigned int i = 0; i < locCount; i++) {
			unsigned int n_ad_i = 0;
			unsigned int* temp = new unsigned int[locCount];
			for (unsigned int j = 0; j < locCount; j++) {
				if (m->getDR(i,j)) {
					temp[n_ad_i++] = j;
				}
			}
			n_ad[i] = n_ad_i;
			ad[i] = new LocationRandomAccess();
			for (unsigned int k = 0; k < n_ad_i; k++) {
				ad[i]->push_back(temp[k]);
			}
			delete[] temp;
		}
	}
	
	unsigned int Adjacents::getAdjacentsCount(unsigned int i) const {
		return n_ad[i];
	}
	const LocationRandomAccess* Adjacents::getAdjacents(unsigned int i) const {
		return ad[i];
	}

}
