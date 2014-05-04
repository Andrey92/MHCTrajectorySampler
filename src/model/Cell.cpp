#include "../include/model/Cell.h"

namespace model {

	/*
	 * Implementation of Cell class.
	 */

	// Cell size value depends on conf file.
	double Cell::size;

	Cell::Cell(unsigned int id) : id(id), center() { }

	Cell::Cell(unsigned int id, double x, double y) : id(id), center(x,y) { }

	Cell::Cell(unsigned int id, const Point& c) : id(id), center(c) { }

	Cell::Cell(const Cell& c) : id(c.id), center(c.center) { }

	double Cell::getSize(void) {
		return size;
	}

	unsigned int Cell::getId(void) const {
		return id;
	}

	const Point& Cell::getCenter(void) const {
		return center;
	}

}
