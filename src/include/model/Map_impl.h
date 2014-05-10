#ifndef MAP_IMPL_H
#define MAP_IMPL_H

#include <algorithm>

namespace model {

	/*
	 * Implementation of template methods:
	 * needed at compile time.
	 */

	template<class LSet, class RSet>
	Map* Map::createMap(const LSet& l, const RSet& r) {
		Map* m = new Map();
		m->width = 0;
		m->height = 0;
		m->initLocations<LSet>(l);
		m->initReaders<RSet>(r);
		m->initConstraints();
		m->initCells();
		m->initDetectionRates();
		return m;
	}

	template<class LSet>
	void Map::initLocations(const LSet& l) {
		lCount = l.size();
		locations = new Location[lCount];
		for (auto it = l.cbegin(); it != l.cend(); it++) {
			Location curr = *it;
			locations[curr.getId()] = curr;
			double newWidth = curr.getPos().getX() + curr.getWidth();
			double newHeight = curr.getPos().getY() + curr.getHeight();
			if (newWidth > width) width = newWidth;
			if (newHeight > height) height = newHeight;
		}
	}

	template<class RSet>
	void Map::initReaders(const RSet& r) {
		rCount = r.size();
		readers = new Reader[rCount];
		for (auto it = r.cbegin(); it != r.cend(); it++) {
			Reader curr = *it;
			readers[curr.getId()] = curr;
		}
	}

	template<class RSet>
	Map::LocationSet* Map::likelyLocations(const RSet& rs) const {
		auto it = rs.cbegin();
		LocationSet* ls = likelyLocations(*it);
		it++;
		for ( ; it != rs.cend(); it++) {
			LocationSet* tmp = likelyLocations(*it);
			for (auto l_it = ls->begin(); l_it != ls->end(); ) {
				Location l = *l_it;
				bool found = false;
				for (auto l_it2 = tmp->begin(); l_it2 != tmp->end(); l_it2++) {
					Location l2 = *l_it2;
					if (l.getId() == l2.getId()) {
						found = true;
						break;
					}
				}
				if (!found) {
					l_it = ls->erase(l_it);
				} else {
					l_it++;
				}
			}
			delete tmp;
		}
		for (auto l_it = ls->begin(); l_it != ls->end(); ) {
			if (!existsCommonCell(rs, *l_it)) {
				l_it = ls->erase(l_it);
			} else {
				l_it++;
			}
		}
		return ls;
	}

	template<class RSet>
	bool Map::existsCommonCell(const RSet& rs, const Location& l) const {
		for (auto it = l.begin(); it != l.end(); it++) {
			bool found = true;
			for (auto r_it = rs.cbegin(); r_it != rs.cend(); r_it++) {
				if (p[r_it->getId()][it->getId()] == 0.0) {
					found = false;
					break;
				}
			}
			if (found) return true;
		}
		return false;
	}

	template<class RSet>
	double Map::prob(const RSet& rs, const Location& l) const {
		double p_tot = 0, pr;
		for (auto c_it = l.begin(); c_it != l.end(); c_it++) {
			Cell c = *c_it; pr = 1;
			for (unsigned int i = 0; i < rCount; i++) {
				Reader r = readers[i];
				if (find(rs.cbegin(), rs.cend(), r) != rs.cend()) {
					pr *= p[r.getId()][c.getId()];
				} else {
					pr *= 1.0 - p[r.getId()][c.getId()];
				}
			}
			p_tot += pr;
		}
		return p_tot / l.cellCount();
	}

}

#endif
