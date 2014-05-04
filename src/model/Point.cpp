#include <cmath>

#include "../include/model/Point.h"

namespace model {

	/*
	 * Implementation of Point class.
	 */

	Point::Point() : x(0), y(0) { }

	Point::Point(double x, double y) : x(x), y(y) { }

	Point::Point(const Point& p) : x(p.x), y(p.y) { }

	double Point::getX(void) const {
		return x;
	}

	double Point::getY(void) const {
		return y;
	}

	double Point::distance(const Point& p) const {
		return hypot(x - p.x, y - p.y);
	}

}
