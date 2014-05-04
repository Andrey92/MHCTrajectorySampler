#include <cmath>
#include <stdexcept>

#include "../include/model/Map.h"
#include "../include/model/Cell.h"
#include "../include/util/Util.h"

using namespace std;
using namespace util;

namespace model {

	/*
	 * Implementation of Map class.
	 */

	void Map::initConstraints(void) {
		dr = Util::allocateMatrix<bool>(lCount, lCount);
		tt = Util::allocateMatrix<unsigned int>(lCount, lCount);
		Util::fillMatrix<bool>(dr, lCount, lCount, false);
		Util::fillMatrix<unsigned int>(tt, lCount, lCount, 0);
	}

	void Map::initCells(void) {
		unsigned int count = 0;
		double cSize = Cell::getSize();
		for (double x = cSize / 2.0; x < width; x += cSize) {
			for (double y = cSize / 2.0; y < height; y += cSize) {
				Location* l = getLocation(x, y);
				if (l != nullptr) {
					l->add(Cell(count++, x, y));
				}
			}
		}
		cCount = count;
	}

	Location* Map::getLocation(double x, double y) const {
		for (unsigned int i = 0; i < lCount; i++) {
			if (locations[i].contains(x, y)) {
				return &locations[i];
			}
		}
		return nullptr;
	}

	void Map::initDetectionRates(void) {
		p = Util::allocateMatrix<double>(rCount, cCount);
		Util::fillMatrix<double>(p, rCount, cCount, 0.0);
		for (unsigned int i = 0; i < lCount; i++) {
			Location l = locations[i];
			for (auto it = l.begin(); it != l.end(); it++) {
				Cell c = *it;
				for (unsigned int j = 0; j < rCount; j++) {
					Reader r = readers[j];
					p[r.getId()][c.getId()] = r.getRate(c.getCenter());
				}
			}
		}
	}

	Map::~Map() {
		delete[] locations;
		delete[] readers;
		Util::deallocateMatrix<bool>(dr);
		Util::deallocateMatrix<unsigned int>(tt);
		Util::deallocateMatrix<double>(p);
	}

	unsigned int Map::getLocationCount(void) const {
		return lCount;
	}

	unsigned int Map::getReaderCount(void) const {
		return rCount;
	}

	unsigned int Map::getCellCount(void) const {
		return cCount;
	}

	double Map::getWidth(void) const {
		return width;
	}

	double Map::getHeight(void) const {
		return height;
	}

	bool Map::getDR(unsigned int l1, unsigned int l2) const {
		if (!isValid(l1) || !isValid(l2)) throw invalid_argument("Invalid id");
		return dr[l1][l2];
	}

	bool Map::getDU(unsigned int l1, unsigned int l2) const {
		if (!isValid(l1) || !isValid(l2)) throw invalid_argument("Invalid id");
		return !dr[l1][l2];
	}

	unsigned int Map::getLT(unsigned int l) const {
		if (!isValid(l)) throw invalid_argument("Invalid id");
		return locations[l].getLatency();
	}

	unsigned int Map::getTT(unsigned int l1, unsigned int l2) const {
		if (!isValid(l1) || !isValid(l2)) throw invalid_argument("Invalid id");
		return tt[l1][l2];
	}

	void Map::setDR(unsigned int l1, unsigned int l2) {
		if (!isValid(l1) || !isValid(l2)) throw invalid_argument("Invalid id");
		dr[l1][l2] = true;
	}

	void Map::setDU(unsigned int l1, unsigned int l2) {
		if (!isValid(l1) || !isValid(l2)) throw invalid_argument("Invalid id");
		dr[l1][l2] = false;
	}

	void Map::setLT(unsigned int l, unsigned int lt) {
		if (!isValid(l) || lt < 0) throw invalid_argument("Invalid id or lt value");
		locations[l].setLatency(lt);
	}

	void Map::setTT(unsigned int l1, unsigned int l2, unsigned int ttvalue) {
		if (!isValid(l1) || !isValid(l2) || tt < 0) throw invalid_argument("Invalid id or tt value");
		tt[l1][l2] = ttvalue;
	}

	bool Map::isValid(unsigned int id) const {
		return (id >= 0 && id < lCount);
	}

	Map::LocationSet* Map::likelyLocations(const Reader& r) const {
		Map::LocationSet* ls = new Map::LocationSet();
		for (unsigned int i = 0; i < lCount; i++) {
			Location l = locations[i];
			if (minDistance(l, r) < r.getMaxRadius()) {
				ls->push_back(l);
			}
		}
		return ls;
	}

	double Map::minDistance(const Location& l, const Reader& r) const {
		double x = r.getPos().getX(), y = r.getPos().getY(),
			   lx = l.getPos().getX(), ly = l.getPos().getY(),
			   fx = lx + l.getWidth(), fy = ly + l.getHeight();
		double px = (x < lx ? lx : (x < fx ? x : fx)),
			   py = (y < ly ? ly : (y < fy ? y : fy));
		return hypot(x - px, y - py);
	}

	Map::location_iterator Map::lbegin(void) const {
		return locations;
	}

	Map::location_iterator Map::lend(void) const {
		return locations + lCount;
	}

	Map::reader_iterator Map::rbegin(void) const {
		return readers;
	}

	Map::reader_iterator Map::rend(void) const {
		return readers + rCount;
	}


}
